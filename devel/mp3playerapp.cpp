#include "mp3playerapp.h"

#include <qwidget.h>


#include <iostream.h>


using namespace std;



Mp3PlayerApp::Mp3PlayerApp( void ) {
    m_is_playing = false;
    m_playing_paused = false;


}


Mp3PlayerApp::~Mp3PlayerApp() {

}


void Mp3PlayerApp::play_slot(void) {
    if(m_is_playing) {
        if(m_playing_paused) {
//           pause_playback(true);
            m_playing_paused = true;
        }
        else {
//           pause_playback(false);
            m_playing_paused = false;
        }
    }
    else {
        m_is_playing = true;
//      play()
    }

}


void Mp3PlayerApp::time_played(int time) {
  int hrs, mins, secs;

  if(time < 0) time = 0;

  hrs = time / (60 * 60);
  time -= hrs * 60 * 60;
  mins = time / 60;
  time -= mins * 60;
  secs = time;
  QString dummy = QString().setNum(hrs) + ":" + QString().setNum(mins) + ":" + QString().setNum(secs);
  played->setText(dummy);

}


void Mp3PlayerApp::total_time(int time) {
  int hrs, mins, secs;

  if(time < 0) time = 0;

  hrs = time / (60 * 60);
  time -= hrs * 60 * 60;
  mins = time / 60;
  time -= mins * 60;
  secs = time;
  QString dummy = QString().setNum(hrs) + ":" + QString().setNum(mins) + ":" + QString().setNum(secs);
  total->setText(dummy);

}

void Mp3PlayerApp::next_slot(){
}

void Mp3PlayerApp::prev_slot(){
}

void Mp3PlayerApp::set_pos_slot(int pos){
  cout << "Slider to pos: " << pos << endl;
}

void Mp3PlayerApp::stop_slot(){
    m_is_playing = false;
    m_playing_paused = false;
}
