#ifndef NEMA_H
#define NEMA_H




#include <qfile.h>
#include <qstring.h>

#include <pthread.h>

#include <qobject.h>
#include <string>

#include "cgpspos.h"

typedef void* (*start_routine_ptr)(void*);

class CNema : public QObject {

    Q_OBJECT;

public:
    CNema(QString filename, int speed = 0);
    ~CNema(void);

    int OpenInterface(QString interface_filename, int speed = 0);

    void CloseInterface(void);

    inline int Baudrate(void) { return m_interface_speed; };

    inline void Baudrate(int speed) { m_interface_speed = speed; };

signals:

    void PosChanged(CGpsPos *pos);
    void NumSatellitesChanged(int num);
    void SpeedAndCourseChanged(float speed, float course);
    void FixIndicatorChanged(int fix_indicator);


private:

    int StartThread(void);

    int StopThread(void);

    static int ThreadMainLoop(CNema* cnema_object);

    void ReadLineFromInterface(void);

    void ParseLine(void);

    inline bool Run(void) { return m_run_thread; };

    inline void Run(bool run) { m_run_thread = run; };


    int ParseGPGGA(void);
    int ParseGPGSA(void);
    int ParseGPRMC(void);
    int ParseGPGSV(void);
    int ParseGGSV(void);
    int ParseGPSA(void);
    int ParseGPR(void);

    CGpsPos m_gps_pos;

    float m_speed;
    float m_course;

    QString m_filename;
    QString m_line;

    pthread_t m_thread_id;
    pthread_attr_t m_thread_attr;

    bool m_run_thread;
    bool m_interface_open;

    int m_interface_speed;

    int m_interface_descr;
    int m_number_of_satellites_used;
};



#endif
