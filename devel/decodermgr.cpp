/**********************************************************
*
*  Mp3PlayerApp, the actual mp3player
*  author: Martin Runge  email  martin <dot> runge <at> web <dot> de
*
**********************************************************/


#include <qvariant.h>
#include <qwidget.h>
#include <qlabel.h>

#include "decodermgr.h"

#include <qpe/mediaplayerplugininterface.h>

static QValueList<MediaPlayerPlugin> DecoderMgr::m_pluginList;

DecoderMgr::DecoderMgr() {
    ScanForCodecs("/mnt/cf/mp3player/codecs");
}

DecoderMgr::~DecoderMgr() {

}

void DecoderMgr::NewDecoder(int index) {

}

int DecoderMgr::ScanForCodecs(QString path) {

    QValueList<MediaPlayerPlugin>::Iterator mit;
    for ( mit = m_pluginList.begin(); mit != m_pluginList.end(); ++mit ) {
        (*mit).iface->release();
        (*mit).library->unload();
        delete (*mit).library;
    }
    m_pluginList.clear();

    QDir dir( path, "lib*.so" );
    QStringList list = dir.entryList();
    QStringList::Iterator it;
    for ( it = list.begin(); it != list.end(); ++it ) {
        MediaPlayerPluginInterface *iface = 0;
        QLibrary *lib = new QLibrary( path + "/" + *it );

        // MediaPlayerDebug(( "querying: %s", QString( path + "/" + *it ).latin1() ));

        if ( lib->queryInterface( IID_MediaPlayerPlugin, (QUnknownInterface**)&iface ) == QS_OK ) {

            // MediaPlayerDebug(( "loading: %s", QString( path + "/" + *it ).latin1() ));

            MediaPlayerPlugin plugin;
            plugin.library = lib;
            plugin.iface = iface;
            plugin.decoder = plugin.iface->decoder();
            plugin.encoder = plugin.iface->encoder();
            m_pluginList.append( plugin );

            // ### hack to get true sample count
            if ( plugin.decoder->pluginName() == QString("LibMpeg3Plugin") )
                libmpeg3decoder = plugin.decoder;

        }
        else {
            delete lib;
        }
    }

    return 0;
}

