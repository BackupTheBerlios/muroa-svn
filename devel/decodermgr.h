/**********************************************************
*
*  Mp3PlayerApp, the actual mp3player
*  author: Martin Runge  email  martin <dot> runge <at> web <dot> de
*
**********************************************************/

#ifndef DECODERMGR_H
#define DECODERMGR_H

#include <qvariant.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qstring.h>


class MediaPlayerDecoder;
class MediaPlayerEncoder;


struct MediaPlayerPlugin {
#ifndef QT_NO_COMPONENT
    QLibrary *library;
#endif
    MediaPlayerPluginInterface *iface;
    MediaPlayerDecoder *decoder;
    MediaPlayerEncoder *encoder;
};


class DecoderMgr
{
    Q_OBJECT

public:
    DecoderMgr( void );
    ~DecoderMgr();

    int ScanForCodecs(QString dir);

public slots:
    void NewDecoder(int index);

private:

    MediaPlayerDecoder* m_cur_decoder;



};


#endif
