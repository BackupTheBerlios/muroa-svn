/**********************************************************
*
*  Mp3PlayerApp, the actual mp3player
*  author: Martin Runge  email  martin <dot> runge <at> web <dot> de
*
**********************************************************/

#ifndef MP3PLAYERAPP_H
#define MP3PLAYERAPP_H

#include <qvariant.h>
#include <qwidget.h>
#include <qlabel.h>

#include "mp3player.h"



class Mp3PlayerApp : public Mp3Player
{
    Q_OBJECT

public:
    Mp3PlayerApp( void );
    ~Mp3PlayerApp();


public slots:
    void next_slot();
    void prev_slot();
    void set_pos_slot(int pos);
    void stop_slot();
    void play_slot();
    void time_played(int time);
    void total_time(int time);

private:
    bool m_is_playing;
    bool m_playing_paused;

};


#endif
