#ifndef NAVAPP_H
#define NAVAPP_H

#include <qmainwindow.h>
#include <qstring.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qtextstream.h>
#include <qwidgetstack.h>
#include <qpe/config.h>
#include <qpe/filemanager.h>
#include <qpe/applnk.h>
#include <qpe/fileselector.h>


#include "cgpspos.h"
#include "nav.h"
#include "nema.h"



class CNavApp : public QMainWindow {

        Q_OBJECT
    public:
        CNavApp(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~CNavApp();
//        Nav* m_nav_widget;

        int deviceChanged(const QString& device_name);
        void bitrateChanged(int speed);


    signals:
        void LatitudeChanged(const QString &latitude);
        void LongitudeChanged(const QString &longitude);
        void AltChanged(const QString &alt);

        void NumSatsChanged(const QString &);

        void SpeedChanged(const QString &);
        void CourseChanged(const QString &);

        void DistanceChanged(const QString &);



//        void UpdatePos(void);
//        void UpdateNumSats(void);


    public slots:
        void PosChanged(CGpsPos* pos);
        void NumSatellitesChanged(int num);
        void SpeedAndCourseChanged(float speed, float course);
//        void FixIndicatorChanged(int fix_indicator);
        void UpdateDisplay(void);
        void setDocument(const QString&);
        void OpenTrack(void);
        void SaveTrack(void);
        void CloseFileSelector(void);
        void FileSelected(const DocLnk &);
        void NewSelected(const DocLnk &);
        void AboutToShowWidget(QWidget* widget);


    private:
        Nav* m_nav_widget;

        CNema * m_nema;
        CGpsPos m_actual_pos;
        float m_speed;
        float m_course;
        int m_num_sats;

        const float knots_to_kmh;

        float m_distance;

        float m_m_per_deg_lat;
        float m_m_per_deg_long;

        int m_earth_radius;


        QTimer* m_display_update_timer;

        Config* m_config;

        QString m_serial_device;
        int m_serial_device_bitrate;

        FileManager m_fm;
        FileSelector *m_file_selector;
        QWidgetStack *m_widget_stack;

        DocLnk *m_current_doclnk;
        QIODevice *m_file_io;
        QTextStream* m_output_stream;


};




#endif
