#ifndef MUROAGUI_H
#define MUROAGUI_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include "CCollection.h"

#include "CDiffBuilder.h"

#include "CPlaylistModel.h"
#include "CCollectionModel.h"
#include "ui_muroagui.h"
#include "CConnection.h"

class CMuroaGui : public QMainWindow
{
    Q_OBJECT

public:
    CMuroaGui(QWidget *parent = 0);
    ~CMuroaGui();

public slots:
    void openConnection();

    void connectionStatusChanged(QString status);


private:
    Ui::muroaguiClass ui;
    CConnection m_connection;

    CCollection<CCollectionItem*> m_collection;
    CCollection<CPlaylistItem*> m_playlist;
    CCollection<CPlaylistItem*> m_nextlist;

    CDiffBuilder m_diffBuilder;

    CCollectionModel m_collectionModel;
    CPlaylistModel m_playlistModel;
    CPlaylistModel m_nextlistModel;

    QLabel m_connection_status_label;

};

#endif // MUROAGUI_H