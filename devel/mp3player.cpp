/****************************************************************************
** Form implementation generated from reading ui file 'mp3player.ui'
**
** Created: Fri Sep 6 21:06:51 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "mp3player.h"

#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"32 32 2 1",
". c None",
"# c #000000",
"................................",
"................................",
"..##............................",
"..####..........................",
"..######........................",
"..########......................",
"..##########....................",
"..############..................",
"..#############.................",
"..###############...............",
"..#################.............",
"..###################...........",
"..#####################.........",
"..#######################.......",
"..#########################.....",
"..##########################....",
"..###########################...",
"..#########################.....",
"..#######################.......",
"..#####################.........",
"..###################...........",
"..#################.............",
"..###############...............",
"..#############.................",
"..###########...................",
"..#########.....................",
"..#######.......................",
"..#####.........................",
"..###...........................",
"..#.............................",
"................................",
"................................"};

static const char* const image1_data[] = { 
"32 32 2 1",
". c None",
"# c #000000",
"................................",
"................................",
"................................",
"................................",
"................................",
"................................",
"................................",
"................................",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"........#################.......",
"................................",
"................................",
"................................",
"................................",
"................................",
"................................",
"................................"};

static const char* const image2_data[] = { 
"32 32 2 1",
". c None",
"# c #000000",
"................................",
"................................",
"................................",
"................................",
"................................",
"...#..........#..........##.....",
"...##.........##.........##.....",
"...###........###........##.....",
"...####.......####.......##.....",
"...####.......####.......##.....",
"...#####......#####......##.....",
"...######.....######.....##.....",
"...#######....#######....##.....",
"...#######....#######....##.....",
"...########...########...##.....",
"...#########..#########..##.....",
"...##########.##########.##.....",
"...#########..#########..##.....",
"...########...########...##.....",
"...#######....#######....##.....",
"...######.....######.....##.....",
"...#####......#####......##.....",
"...#####......#####......##.....",
"...####.......####.......##.....",
"...###........###........##.....",
"...##.........##.........##.....",
"...#..........#..........##.....",
"................................",
"................................",
"................................",
"................................",
"................................"};

static const char* const image3_data[] = { 
"32 32 2 1",
". c None",
"# c #000000",
"................................",
"................................",
"................................",
"......##.........#.........#....",
"......##.........#.........#....",
"......##........##........##....",
"......##.......###.......###....",
"......##.......###.......###....",
"......##......####......####....",
"......##.....#####.....#####....",
"......##.....#####.....#####....",
"......##....######....######....",
"......##...#######...#######....",
"......##...#######...#######....",
"......##..########..########....",
"......##.#########.#########....",
"......##.#########.#########....",
"......##..########..########....",
"......##..########..########....",
"......##...#######...#######....",
"......##....######....######....",
"......##....######....######....",
"......##.....#####.....#####....",
"......##.....#####.....#####....",
"......##......####......####....",
"......##.......###.......###....",
"......##.......###.......###....",
"......##........##........##....",
"......##.........#.........#....",
"................................",
"................................",
"................................"};


/* 
 *  Constructs a Mp3Player which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
Mp3Player::Mp3Player( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    QPixmap image2( ( const char** ) image2_data );
    QPixmap image3( ( const char** ) image3_data );
    if ( !name )
	setName( "Mp3Player" );
    resize( 232, 315 ); 
    setMaximumSize( QSize( 240, 320 ) );
    setCaption( tr( "Example" ) );
    setIcon( image0 );

    tabbar = new QTabWidget( this, "tabbar" );
    tabbar->setGeometry( QRect( 0, 60, 240, 260 ) ); 

    tab = new QWidget( tabbar, "tab" );

    songlist = new QListView( tab, "songlist" );
    songlist->addColumn( tr( "Artist" ) );
    songlist->addColumn( tr( "Title" ) );
    songlist->addColumn( tr( "length" ) );
    songlist->setGeometry( QRect( 0, 0, 240, 230 ) ); 
    tabbar->insertTab( tab, tr( "Songlist" ) );

    tab_2 = new QWidget( tabbar, "tab_2" );

    playlist = new QListView( tab_2, "playlist" );
    playlist->addColumn( tr( "Artist" ) );
    playlist->addColumn( tr( "Title" ) );
    playlist->addColumn( tr( "length" ) );
    playlist->setGeometry( QRect( 0, 0, 240, 230 ) ); 
    tabbar->insertTab( tab_2, tr( "Playlist" ) );

    stop = new QPushButton( this, "stop" );
    stop->setGeometry( QRect( 80, 0, 41, 41 ) ); 
    stop->setText( tr( "" ) );
    stop->setPixmap( image1 );

    next = new QPushButton( this, "next" );
    next->setGeometry( QRect( 120, 0, 41, 41 ) ); 
    next->setText( tr( "" ) );
    next->setPixmap( image2 );

    prev = new QPushButton( this, "prev" );
    prev->setGeometry( QRect( 0, 0, 40, 40 ) ); 
    prev->setText( tr( "" ) );
    prev->setPixmap( image3 );

    played = new QLabel( this, "played" );
    played->setGeometry( QRect( 161, 0, 80, 20 ) ); 
    played->setText( tr( "played" ) );

    play = new QPushButton( this, "play" );
    play->setGeometry( QRect( 40, 0, 40, 40 ) ); 
    play->setText( tr( "" ) );
    play->setPixmap( image0 );

    total = new QLabel( this, "total" );
    total->setGeometry( QRect( 160, 20, 60, 20 ) ); 
    total->setText( tr( "total" ) );

    Slider1 = new QSlider( this, "Slider1" );
    Slider1->setGeometry( QRect( 0, 41, 240, 20 ) ); 
    Slider1->setOrientation( QSlider::Horizontal );

    // signals and slots connections
    connect( play, SIGNAL( clicked() ), this, SLOT( play_slot() ) );
    connect( stop, SIGNAL( clicked() ), this, SLOT( stop_slot() ) );
    connect( next, SIGNAL( clicked() ), this, SLOT( next_slot() ) );
    connect( prev, SIGNAL( clicked() ), this, SLOT( prev_slot() ) );
    connect( Slider1, SIGNAL( sliderMoved(int) ), this, SLOT( set_pos_slot(int) ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
Mp3Player::~Mp3Player()
{
    // no need to delete child widgets, Qt does it all for us
}

void Mp3Player::next_slot()
{
    qWarning( "Mp3Player::next_slot(): Not implemented yet!" );
}

void Mp3Player::play_slot()
{
    qWarning( "Mp3Player::play_slot(): Not implemented yet!" );
}

void Mp3Player::prev_slot()
{
    qWarning( "Mp3Player::prev_slot(): Not implemented yet!" );
}

void Mp3Player::set_pos_slot(int)
{
    qWarning( "Mp3Player::set_pos_slot(int): Not implemented yet!" );
}

void Mp3Player::stop_slot()
{
    qWarning( "Mp3Player::stop_slot(): Not implemented yet!" );
}

void Mp3Player::time_played(int time)
{
    qWarning( "Mp3Player::time_played(int time): Not implemented yet!" );
}

void Mp3Player::total_time(int time)
{
    qWarning( "Mp3Player::total_time(int time): Not implemented yet!" );
}

