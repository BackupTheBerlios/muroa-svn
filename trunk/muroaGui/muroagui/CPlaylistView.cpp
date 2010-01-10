/*
 * CPlaylistView.cpp
 *
 *  Created on: 6 Jan 2010
 *      Author: martin
 */
#include <QtGui>
#include <QDebug>
#include <QMimeData>

#include "CPlaylistView.h"
#include "CPlaylistModel.h"
#include "CPlaylistItem.h"

#include "CModelDiff.h"

CPlaylistView::CPlaylistView(QWidget * parent ) : QListView( parent ), m_dragActive(false){
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	//setDragDropMode( QAbstractItemView::DragDrop );

}

CPlaylistView::~CPlaylistView() {
	// TODO Auto-generated destructor stub
}

void CPlaylistView::mousePressEvent(QMouseEvent *event)
{
	qDebug() << QString("mousePressEvent");
    if (event->button() == Qt::LeftButton && !m_dragActive)
        m_startPos = event->pos();
    QListView::mousePressEvent(event);
}

void CPlaylistView::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << QString("mouseMoveEvent");
    if (event->buttons() & Qt::LeftButton && !m_dragActive) {
        int distance = (event->pos() - m_startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    QListView::mouseMoveEvent(event);
}

void CPlaylistView::dragEnterEvent(QDragEnterEvent *event)
{
	qDebug() << QString("dragEnterEvent");
    //CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CPlaylistView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << QString("dragMoveEvent");
    // CPlaylistView *source = qobject_cast<CPlaylistView *>(event->source());
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void CPlaylistView::dropEvent(QDropEvent *event)
{
	qDebug() << QString("dropEvent");
	const QMimeData* data = event->mimeData();

    if (data && data->hasFormat("application/x-muroa-playlist-diff")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        CModelDiff md(data->data("application/x-muroa-playlist-diff"));
        QModelIndex currentIdx = indexAt( event->pos());
        md.appendRowToInsert(currentIdx.row());

        qDebug() << QString("Move %1 to %2").arg(md.getRowsToRemove().at(0)).arg(md.getRowsToInsert().at(0));

    }
}

void CPlaylistView::performDrag()
{
	qDebug() << QString("performDrag");

    CPlaylistModel* plModel = reinterpret_cast<CPlaylistModel*>(model());

    QModelIndexList indexList = selectedIndexes();
    CModelDiff md;
    md.setRowsToRemove(indexList);

    if (md.getNumRowsToRemove() > 0)
    {
    	QMimeData *mimeData = new QMimeData;
        //mimeData->setText(item->asString());
        mimeData->setData("application/x-muroa-playlist-diff", md.toQByteArray());

        m_dragActive = true;
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        // drag->setPixmap(QPixmap(":/images/person.png"));

        Qt::DropAction action = drag->exec(Qt::MoveAction);

        m_dragActive = false;
        qDebug() << QString("DropAction: %1").arg(action);

    }
}
