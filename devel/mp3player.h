/****************************************************************************
** Form interface generated from reading ui file 'mp3player.ui'
**
** Created: Fri Sep 6 21:06:44 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QSlider;
class QTabWidget;

class Mp3Player : public QWidget
{ 
    Q_OBJECT

public:
    Mp3Player( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Mp3Player();

    QTabWidget* tabbar;
    QWidget* tab;
    QListView* songlist;
    QWidget* tab_2;
    QListView* playlist;
    QPushButton* stop;
    QPushButton* next;
    QPushButton* prev;
    QLabel* played;
    QPushButton* play;
    QLabel* total;
    QSlider* Slider1;

public slots:
    virtual void next_slot();
    virtual void play_slot();
    virtual void prev_slot();
    virtual void set_pos_slot(int);
    virtual void stop_slot();
    virtual void time_played(int time);
    virtual void total_time(int time);

};

#endif // MP3PLAYER_H
