
#include <qstring.h>
#include <qthread.h>
#include <math.h>
#include <unistd.h>

#include <qpe/qcopenvelope_qws.h>
#include <qpe/qpeapplication.h>

#include "cnavapp.h"




CNavApp::CNavApp(QWidget* parent = 0, const char* name = 0, WFlags fl = 0)
    : QMainWindow(parent, name, fl),
     knots_to_kmh(1.852),
     m_earth_radius(6371040)  {

    QString dummy;

    m_current_doclnk = 0;

    m_m_per_deg_lat = m_earth_radius * M_PI / 180;
    m_m_per_deg_long = m_earth_radius * cos(48 * M_PI / 180) * M_PI / 180;

    m_distance = 0;
    m_file_selector = 0;

    m_widget_stack = new QWidgetStack( this );
    m_widget_stack->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    setCentralWidget( m_widget_stack );


 //   m_file_selector = new FileSelector( "text/*", m_widget_stack, "fileselector" , TRUE, TRUE );
 //   connect(m_file_selector, SIGNAL( closeMe() ), this, SLOT( CloseFileSelector() ) );
 //   connect(m_file_selector, SIGNAL( fileSelected(const DocLnk &)), this, SLOT( FileSelected(const DocLnk &) ) );
 //   connect(m_file_selector, SIGNAL( newSelected(const DocLnk &)), this, SLOT( NewSelected(const DocLnk &) ) );

    connect(m_widget_stack, SIGNAL( aboutToShow( QWidget* )), this, SLOT( AboutToShowWidget(QWidget*)));

    cout << "m_widget_stack = " << hex << m_widget_stack << dec << endl;

//    m_file_selector->reread();

//    cerr << "m_file_selctor(" << hex << m_file_selector << dec << ")" << endl;


//    m_current_doclnk = new DocLnk("/opt/Qtopia/apps/Applications/nav.desktop");
    m_current_doclnk = new DocLnk("/home/martin/devel/nav/test.track");
//    if(m_fm.exists(*m_current_doclnk)) {
//        m_file_io = m_fm.openFile(*m_current_doclnk);
//
//        delete m_file_io;
//    }

    if(m_current_doclnk) {
        cout << "DocLnk created" << endl;
    }
    else {
        cout << "failed to create DocLnk" << endl;
    }
    m_current_doclnk->setFile("/home/root/Documents/test.track");
//    m_current_doclnk->setType("text/plain");
    m_file_io = m_fm.saveFile(*m_current_doclnk);
    if(m_file_io == 0) {
        cout << "Error opening file" << endl;
    }

//   m_file_io->open("/home/martin/devel/nav/track1.track");
    m_output_stream = new QTextStream(m_file_io);

    cout << "Los gehts" << endl;


    m_config = new Config("NavConfig");
    m_config->setGroup("MainConfig");

    m_serial_device = m_config->readEntry("DeviceFile", "/dev/ttyS3" );

    m_serial_device_bitrate = m_config->readNumEntry("DeviceBitrate", 4800 );

    m_nema = new CNema(m_serial_device, m_serial_device_bitrate);

    m_nav_widget = new Nav(m_widget_stack);

    cerr << "m_nav_widget(" << hex << m_nav_widget << dec << ")" << endl;

    QCopEnvelope("QPE/System","setScreenSaverMode(int)")<<QPEApplication::DisableSuspend;


//    m_nav_widget->setGeometry( QRect( 0, 0, 240, 280 ) );
    connect( m_nema, SIGNAL(PosChanged(CGpsPos*)), this, SLOT(PosChanged(CGpsPos*)));
    connect( m_nema, SIGNAL(NumSatellitesChanged(int)), this, SLOT(NumSatellitesChanged(int)));

    connect( m_nema, SIGNAL(SpeedAndCourseChanged(float, float)), (QObject*)this, SLOT(SpeedAndCourseChanged(float, float)));

    connect( (QObject*)this,SIGNAL(LatitudeChanged(const QString&)), (QObject*)(m_nav_widget->Latitude), SLOT(setText(const QString &)));
    connect( (QObject*)this,SIGNAL(LongitudeChanged(const QString&)), (QObject*)(m_nav_widget->Longitude), SLOT(setText(const QString &)));
    connect( (QObject*)this,SIGNAL(AltChanged(const QString&)), (QObject*)(m_nav_widget->Alt), SLOT(setText(const QString &)));

    connect( (QObject*)this,SIGNAL(SpeedChanged(const QString&)), (QObject*)(m_nav_widget->Speed), SLOT(setText(const QString &)));
    connect( (QObject*)this,SIGNAL(CourseChanged(const QString&)), (QObject*)(m_nav_widget->Course), SLOT(setText(const QString &)));

    connect( (QObject*)this,SIGNAL(DistanceChanged(const QString&)), (QObject*)(m_nav_widget->Distance), SLOT(setText(const QString &)));

//    connect ((QObject*)m_nav_widget,SIGNAL(bitrateChanged(int)), (QObject*)this, SLOT(bitrateChanged(int)));
//    connect ((QObject*)m_nav_widget,SIGNAL(deviceChanged(const QString&)), (QObject*)this, SLOT(deviceChanged(const QString&)));

//    connect( (QObject*)this,SIGNAL(UpdatePos(void)), (QObject*)(m_nav_widget->Latitude), SLOT(update(void)));
//    connect( (QObject*)this,SIGNAL(UpdatePos(void)), (QObject*)(m_nav_widget->Longitude), SLOT(update(void)));
//    connect( (QObject*)this,SIGNAL(UpdatePos(void)), (QObject*)(m_nav_widget->Alt), SLOT(update(void)));

//    connect( (QObject*)this,SIGNAL(UpdateNumSats(void)), (QObject*)(m_nav_widget->NumSats), SLOT(update(void)));

    connect( (QObject*)this,SIGNAL(NumSatsChanged(const QString&)), (QObject*)(m_nav_widget->NumSats), SLOT(setText(const QString &)));

    m_display_update_timer = new QTimer(this);

    connect(m_display_update_timer, SIGNAL(timeout()), this, SLOT(UpdateDisplay()));

    m_display_update_timer->start(100);

    m_widget_stack->raiseWidget(m_nav_widget);

}

CNavApp::~CNavApp(void) {

    m_display_update_timer->stop();

//    m_file_io->close();
//    delete m_file_io;


//    delete m_current_doclnk;


    delete m_config;

    delete m_nav_widget;


    delete m_nema;

    QCopEnvelope("QPE/System","setScreenSaverMode(int)")<<QPEApplication::Enable;

}


void CNavApp::PosChanged(CGpsPos* pos) {

    QString str;
    static float lat_base = 0;
    float latdiff, longdiff;

    if(pos->FixIndicator() != 0)  {
        if(m_actual_pos.FixIndicator() != 0) {
            // calculate distance between last valid fix and the actual fix (if valid)
            latdiff = m_actual_pos.Latitude() - pos->Latitude();
            longdiff = m_actual_pos.Longitude() - pos->Longitude();

            if(fabs(lat_base - m_actual_pos.Latitude()) > 0.5 ) {
                m_m_per_deg_long = m_earth_radius * cos(m_actual_pos.Latitude() * M_PI / 180) * M_PI / 180;
            }


            latdiff *= m_m_per_deg_lat;
            longdiff *= m_m_per_deg_long;


            m_distance += fabs(sqrt(latdiff * latdiff + longdiff * longdiff));

        }

        // Write actual pos to member variable if it is from a valid fix
        m_actual_pos = *pos;
        
        //(*m_output_stream) << pos->Latitude() << ", " << pos->Longitude() << ", " << pos->Alt() ", " << m_num_sats << endl;
        (*m_output_stream) << "na was" << endl;

//        *m_output_stream << pos->Latitude() << " , " << pos->Longitude() << " , " <<  pos->Alt() << endl;

    }

    //LatitudeChanged(pos->Latitude());   // pos->Latitude()
    //Longitude->SetText("11.5");   // pos->Longitude()
    //Alt->SetText("135");    // pos->Alt()

}

void CNavApp::setDocument(const QString& doc_name) {

    cout << "setDocument(\"" << doc_name << "\") called " << endl;

}


void CNavApp::NumSatellitesChanged(int num) {

    m_num_sats = num;

    cout << "Num Satellites : " << num << endl;


}

void CNavApp::SpeedAndCourseChanged(float speed, float course) {
    QString str;

    m_speed = speed * knots_to_kmh;

    m_course = course;

}


void CNavApp::UpdateDisplay() {

    static float speed = 0;
    static float course = 0;
    static float latitude = 0;
    static float longitude = 0;
    static float alt = 0;
    static int num_sats = 0;
    static float distance = 0;

    QString str;


    if( latitude != m_actual_pos.Latitude()) {
        latitude = m_actual_pos.Latitude();
        emit LatitudeChanged(str.sprintf("%.6f", latitude));
    }

    if(longitude != m_actual_pos.Longitude()) {
        longitude = m_actual_pos.Longitude();
        emit LongitudeChanged(str.sprintf("%.6f", longitude ));
    }

    if(alt != m_actual_pos.Alt()) {
        alt = m_actual_pos.Alt();
        emit AltChanged(str.sprintf("%.1f", alt));
    }

    if(speed != m_speed) {
        speed = m_speed;
        emit SpeedChanged(str.sprintf("%3.2f km/h", speed));
    }

    if(course != m_course) {
        m_course = course;
        emit CourseChanged(str.sprintf("%3.2f °", course));
    }

    if(m_num_sats != num_sats) {
        num_sats = m_num_sats;
        emit NumSatsChanged(str.sprintf("Sats %2d", num_sats));
    }

    if(m_distance != distance) {
        distance = m_distance;
        emit DistanceChanged(str.sprintf("Distance %6d m", (unsigned long)m_distance));
    }

}

int CNavApp::deviceChanged(const QString& device_name) {
    int retval;
    retval = m_nema->OpenInterface(device_name);
    if(retval == 0) {
        m_config->writeEntry("DeviceFile", device_name);
    }
    return retval;
}

void CNavApp::bitrateChanged(int speed) {

    m_nema->Baudrate(speed);
    m_config->writeEntry("DeviceBitrate", speed);
}



void CNavApp::SaveTrack(void) {

        m_file_selector->reread();

    //    m_file_selector = new FileSelector( "text/*", m_nav_widget, "fileselector" , TRUE, TRUE );
    //    connect(m_file_selector, SIGNAL( closeMe() ), this, SLOT( CloseFileSelector() ) );



}

void CNavApp::OpenTrack(void) {

//    cout << "Open Track. m_file_selector == " << hex << m_file_selector << dec << endl;

//    m_file_selector = new FileSelector( "text/*", m_widget_stack, "fileselector" , TRUE, TRUE );
//    m_file_selector->reread();

    cout << "About to open Track. " << endl;
    cout << "m_widget_stack = " << hex << m_widget_stack << dec << endl;

    m_file_selector = new FileSelector( "text/*", m_widget_stack, "fileselector" , TRUE, TRUE );

    QObject::connect(m_file_selector, SIGNAL( closeMe() ), this, SLOT( CloseFileSelector() ) );
    QObject::connect(m_file_selector, SIGNAL( fileSelected(const DocLnk &)), this, SLOT( FileSelected(const DocLnk &) ) );
    connect(m_file_selector, SIGNAL( newSelected(const DocLnk &)), this, SLOT( NewSelected(const DocLnk &) ) );

//    m_file_selector->reread();

    m_widget_stack->raiseWidget(m_file_selector);

    cout << "Open Track. m_file_selector == " << hex << m_file_selector << dec << endl;

//    m_file_selector->reread();

}


void CNavApp::CloseFileSelector(void) {
    cout << "Closing file selector." << endl;
    m_widget_stack->raiseWidget(m_nav_widget);
    m_widget_stack->removeWidget(m_file_selector);
//    cout << "Nav widget raised. waiting 5 secs." << endl;
//    sleep(5);
//    m_widget_stack->raiseWidget(m_file_selector);
//    cout << "FileSelector widget raised. waiting 5 secs." << endl;
//    sleep(5);
//    m_widget_stack->raiseWidget(m_nav_widget);
//    cout << "Nav widget raised. continuing." << endl;


}


void CNavApp::FileSelected(const DocLnk &file) {
    m_file_io = m_fm.openFile(file);
    if(m_file_io == 0) {
        cout << "Error opening file" << endl;
    }
    cout << "Existing file selected. Filename is " << file.name()  << endl;
//   m_file_io->open("/home/martin/devel/nav/track1.track");
    m_output_stream = new QTextStream(m_file_io);

}


void CNavApp::NewSelected(const DocLnk &file) {
    m_file_io = m_fm.saveFile(file);
    if(m_file_io == 0) {
        cout << "Error opening file" << endl;
    }
    cout << "New file selected. using " << file.name() << " as filename. " << endl;
//   m_file_io->open("/home/martin/devel/nav/track1.track");
    m_output_stream = new QTextStream(m_file_io);
}


void CNavApp::AboutToShowWidget(QWidget* widget) {

    cerr << "AboutToShowWidget(" << hex << widget << dec << ")" << endl;
    if(widget == m_file_selector) {
        cerr << "Trying to reread fileselector." << endl;
        m_file_selector->reread();
    }

}
