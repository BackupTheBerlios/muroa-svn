

#include "nema.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include <iostream.h>

using namespace std;

CNema::CNema(QString filename, int speed = 0) {
    m_interface_open = false;

    m_number_of_satellites_used = -1;

    if(!filename.isEmpty()) {
        OpenInterface(filename, speed);
    }

}

CNema::~CNema(void) {
    CloseInterface();
}

int CNema::OpenInterface(QString filename, int speed = 0) {
    CloseInterface();
    cout << "Opening Interface " << filename << endl;
    m_interface_descr = open((const char*) filename, O_RDWR );
    if(m_interface_descr == -1) {
        // an error occourred
        cout << "error opening interface" << endl;
        return -1;
    }

    if(speed != 0) {
        m_interface_speed = speed;
    }

    int baudrate_def = 0;
    switch(m_interface_speed) {
        case 75:        baudrate_def = B75;       break;
        case 150:       baudrate_def = B150;      break;
        case 300:       baudrate_def = B300;      break;
        case 600:       baudrate_def = B600;      break;
        case 1200:      baudrate_def = B1200;     break;
        case 2400:      baudrate_def = B2400;     break;
        case 4800:      baudrate_def = B4800;     break;
        case 9600:      baudrate_def = B9600;     break;
        case 19200:     baudrate_def = B19200;    break;
        case 38400:     baudrate_def = B38400;    break;
        case 57600:     baudrate_def = B57600;    break;
        case 115200:    baudrate_def = B57600;    break;
        default:        baudrate_def = 0;         return -3;

    }
    struct termios tio_struct;
    tio_struct.c_iflag= IGNBRK|IGNPAR;
    tio_struct.c_oflag= 0;
    tio_struct.c_cflag= CS8|CREAD|CLOCAL;
    tio_struct.c_lflag= 0;
    cfsetispeed(&tio_struct,baudrate_def);
    cfsetospeed(&tio_struct,baudrate_def);
    if(tcsetattr(m_interface_descr,TCSANOW,&tio_struct)<0) {
        cout << "error setting baudrate" << endl;

        return -2;
         // system::error(0,"warning:couldn't configure serial port %s\n",name);
    }

    m_interface_open = true;

    StartThread();

    return 0;
}

void CNema::CloseInterface(void) {
    StopThread();
    if(m_interface_open)
        close(m_interface_descr);

    m_interface_open = false;
}


int CNema::StartThread() {

    int retval;
    Run(true);
    retval = pthread_create(&m_thread_id, NULL, (start_routine_ptr)CNema::ThreadMainLoop, (void*) this);
    return retval;
}

int CNema::StopThread() {
    int retval;

    Run(false);
    retval = pthread_join(m_thread_id, NULL);
    return retval;
}

int CNema::ThreadMainLoop(CNema* cnema_object) {
    int retval = 0;
    while (cnema_object->Run()) {
        cnema_object->ReadLineFromInterface();
    }
    return retval;
}


void CNema::ReadLineFromInterface() {
    const int BUFFERSIZE = 128;

    static char buf[BUFFERSIZE];

    int num_bytes_read;
    int pos;

    QString tmp_string;

    do {

        num_bytes_read = read(m_interface_descr, buf, BUFFERSIZE);

        buf[num_bytes_read] = 0;

        m_line.append(buf);

        pos = m_line.find("\r\n");

    }
    while(pos == -1);

    tmp_string = m_line.right(m_line.length() - pos - 2);

    m_line = m_line.left(pos);

    ParseLine();

    m_line = tmp_string;

    return;
}

/** parses the line currently in m_line */
void CNema::ParseLine() {

    if(m_line.contains("$GPGGA"))
        ParseGPGGA();
    else if(m_line.contains("$GPGSA"))
        ParseGPGSA();
    else if(m_line.contains("$GPRMC"))
        ParseGPRMC();
    else if(m_line.contains("$GPGSV"))
        ParseGPGSV();
    else if(m_line.contains("$GPSA"))
        ParseGPSA();
    else if(m_line.contains("$GPR"))
        ParseGPR();
    else
        cout << "invalid message from GPS \n " << m_line << endl;

}

int CNema::ParseGPGGA(void) {

    int oldpos, pos;
    QString substring;

#ifdef DEBUG
    cout << "ParseGPGGA: m_line= " << m_line << endl;
#endif
    // $GPGGA
    pos = m_line.find(',', 0);
    oldpos = pos + 1;

    // Time in UTC
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Time: " << substring << endl;
#endif
    oldpos = pos + 1;

    int point_pos;
    int degrees;
    int minutes;
    float seconds;
    QString tmpstr;
    // Latitude
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Latitude: " << substring << endl;
#endif
    point_pos = substring.find('.');
    tmpstr = substring.left(point_pos - 2);
    degrees = tmpstr.toInt();

    tmpstr = substring.mid(point_pos - 2, 2);
    minutes = tmpstr.toInt();


    tmpstr = substring.right(substring.length() - point_pos - 1);
    seconds = tmpstr.toFloat() / 100;
#ifdef DEBUG
    cout << "Degrees: " << degrees << " Minutes " << minutes << " Seconds " << seconds << endl;
#endif
    m_gps_pos.Latitude(degrees + (float)minutes / 60 + seconds / (60 * 60));
    oldpos = pos + 1;

    // North/South
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " North/South: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Longitude
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Longitude: " << substring << endl;
#endif
    point_pos = substring.find('.');
    tmpstr = substring.left(point_pos - 2);
    degrees = tmpstr.toInt();

    tmpstr = substring.mid(point_pos - 2, 2);
    minutes = tmpstr.toInt();

    tmpstr = substring.right(substring.length() - point_pos - 1);
    seconds = tmpstr.toFloat() / 100;
#ifdef DEBUG
    cout << "Degrees: " << degrees << " Minutes " << minutes << " Seconds " << seconds << endl;
#endif
    m_gps_pos.Longitude(degrees + (float)minutes / 60 + seconds / (60*60));
    oldpos = pos + 1;

    // East/West
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " East/West: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Fix indicator
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " fix indicator: " << substring << endl;
#endif
    int fix_indicator = substring.toInt();
    m_gps_pos.FixIndicator(fix_indicator);
    oldpos = pos + 1;

    // Number of Satellites
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Number of satellites: " << substring << endl;
#endif
    int tmp_num = substring.toInt();
    if(tmp_num != m_number_of_satellites_used) {
        m_number_of_satellites_used = tmp_num;
        emit NumSatellitesChanged(m_number_of_satellites_used);
    }
    oldpos = pos + 1;

    // unknown value
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " unknown: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Height
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Height: " << substring << endl;
#endif
    m_gps_pos.Alt(substring.toFloat());
    oldpos = pos + 1;

    // unknown value
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " unknown: " << substring << endl;
#endif
    oldpos = pos + 1;

    emit(PosChanged(&m_gps_pos));

    return 0;
}

int CNema::ParseGPGSA(void) {

#ifdef DEBUG
    cout << "ParseGPGSA: m_line= " << m_line << endl;
#endif

    return 0;

}

int CNema::ParseGPRMC(void) {

    int oldpos, pos;
    QString substring;

    int point_pos;
    int degrees;
    int minutes;
    float seconds;
    QString tmpstr;


#ifdef DEBUG
    cout << "ParseGPRMC: m_line= " << m_line << endl;
#endif

    // $GPRMC
    pos = m_line.find(',', 0);
    oldpos = pos + 1;

    // Time in UTC
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Time: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Time in UTC
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Valid or not: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Latitude
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Latitude: " << substring << endl;
#endif
    point_pos = substring.find('.');
    tmpstr = substring.left(point_pos - 2);
    degrees = tmpstr.toInt();

    tmpstr = substring.mid(point_pos - 2, 2);
    minutes = tmpstr.toInt();

    tmpstr = substring.right(substring.length() - point_pos - 1);
    seconds = tmpstr.toFloat() / 100;

    m_gps_pos.Latitude(degrees + (float)minutes / 60 + seconds / (60 * 60));
    oldpos = pos + 1;

    // North/South
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " North/South: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Longitude
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Longitude: " << substring << endl;
#endif
    point_pos = substring.find('.');
    tmpstr = substring.left(point_pos - 2);
    degrees = tmpstr.toInt();

    tmpstr = substring.mid(point_pos - 2, 2);
    minutes = tmpstr.toInt();

    tmpstr = substring.right(substring.length() - point_pos - 1);
    seconds = tmpstr.toFloat() / 100;

    m_gps_pos.Longitude(degrees + (float)minutes / 60 + seconds / (60 * 60));
    oldpos = pos + 1;

    // East/West
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " East/West: " << substring << endl;
#endif
    oldpos = pos + 1;

    // Speed in knots
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Speed: " << substring << " knots" << endl;
#endif
    m_speed = substring.toFloat();
    oldpos = pos + 1;

    // Course
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Course: " << substring << endl;
#endif
    m_course = substring.toFloat();
    emit SpeedAndCourseChanged(m_speed, m_course);
    oldpos = pos + 1;


    // Date
    pos = m_line.find(',', oldpos);
    substring = m_line.mid(oldpos, pos - oldpos);
#ifdef DEBUG
    cout << "Pos " << oldpos << " - " << pos << " Date: " << substring << endl;
#endif
    oldpos = pos + 1;



    return 0;

}


int CNema::ParseGPGSV(void) {

#ifdef DEBUG
    cout << "ParseGPGSV: m_line= " << m_line << endl;
#endif

    return 0;

}


int CNema::ParseGPSA(void) {

#ifdef DEBUG
    cout << "ParseGPSA: m_line= " << m_line << endl;
#endif

    return 0;

}


int CNema::ParseGPR(void) {

#ifdef DEBUG
    cout << "ParseGPR: m_line= " << m_line << endl;
#endif

    return 0;

}

int CNema::ParseGGSV(void) {

#ifdef DEBUG
    cout << "ParseGGSV: m_line= " << m_line << endl;
#endif

    return 0;

}
