/****************************************************************************
** Mp3PlayerApp meta object code from reading C++ file 'mp3playerapp.h'
**
** Created: Fri Sep 6 21:06:56 2002
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.3.3   edited 2001-10-17 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "mp3playerapp.h"
#include <qmetaobject.h>
#include <qapplication.h>



const char *Mp3PlayerApp::className() const
{
    return "Mp3PlayerApp";
}

QMetaObject *Mp3PlayerApp::metaObj = 0;

void Mp3PlayerApp::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(Mp3Player::className(), "Mp3Player") != 0 )
	badSuperclassWarning("Mp3PlayerApp","Mp3Player");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString Mp3PlayerApp::tr(const char* s)
{
    return qApp->translate( "Mp3PlayerApp", s, 0 );
}

QString Mp3PlayerApp::tr(const char* s, const char * c)
{
    return qApp->translate( "Mp3PlayerApp", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* Mp3PlayerApp::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) Mp3Player::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void (Mp3PlayerApp::*m1_t0)();
    typedef void (QObject::*om1_t0)();
    typedef void (Mp3PlayerApp::*m1_t1)();
    typedef void (QObject::*om1_t1)();
    typedef void (Mp3PlayerApp::*m1_t2)(int);
    typedef void (QObject::*om1_t2)(int);
    typedef void (Mp3PlayerApp::*m1_t3)();
    typedef void (QObject::*om1_t3)();
    typedef void (Mp3PlayerApp::*m1_t4)();
    typedef void (QObject::*om1_t4)();
    typedef void (Mp3PlayerApp::*m1_t5)(int);
    typedef void (QObject::*om1_t5)(int);
    typedef void (Mp3PlayerApp::*m1_t6)(int);
    typedef void (QObject::*om1_t6)(int);
    m1_t0 v1_0 = &Mp3PlayerApp::next_slot;
    om1_t0 ov1_0 = (om1_t0)v1_0;
    m1_t1 v1_1 = &Mp3PlayerApp::prev_slot;
    om1_t1 ov1_1 = (om1_t1)v1_1;
    m1_t2 v1_2 = &Mp3PlayerApp::set_pos_slot;
    om1_t2 ov1_2 = (om1_t2)v1_2;
    m1_t3 v1_3 = &Mp3PlayerApp::stop_slot;
    om1_t3 ov1_3 = (om1_t3)v1_3;
    m1_t4 v1_4 = &Mp3PlayerApp::play_slot;
    om1_t4 ov1_4 = (om1_t4)v1_4;
    m1_t5 v1_5 = &Mp3PlayerApp::time_played;
    om1_t5 ov1_5 = (om1_t5)v1_5;
    m1_t6 v1_6 = &Mp3PlayerApp::total_time;
    om1_t6 ov1_6 = (om1_t6)v1_6;
    QMetaData *slot_tbl = QMetaObject::new_metadata(7);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(7);
    slot_tbl[0].name = "next_slot()";
    slot_tbl[0].ptr = (QMember)ov1_0;
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "prev_slot()";
    slot_tbl[1].ptr = (QMember)ov1_1;
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "set_pos_slot(int)";
    slot_tbl[2].ptr = (QMember)ov1_2;
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "stop_slot()";
    slot_tbl[3].ptr = (QMember)ov1_3;
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "play_slot()";
    slot_tbl[4].ptr = (QMember)ov1_4;
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "time_played(int)";
    slot_tbl[5].ptr = (QMember)ov1_5;
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "total_time(int)";
    slot_tbl[6].ptr = (QMember)ov1_6;
    slot_tbl_access[6] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"Mp3PlayerApp", "Mp3Player",
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
