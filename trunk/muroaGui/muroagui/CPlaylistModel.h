/*
 * CPlaylistModel.h
 *
 *  Created on: 1 Dec 2009
 *      Author: martin
 */

#ifndef CPLAYLISTMODEL_H_
#define CPLAYLISTMODEL_H_


#include <QVariant>
#include <QList>

#include "CModelBase.h"
#include "CCollection.h"
#include "CPlaylistItem.h"


class CPlaylistModel : public CModelBase<CPlaylistItem*> {
public:
	CPlaylistModel(QObject* parent = 0);
	virtual ~CPlaylistModel();

	inline void setPlaylist(CCollection<CPlaylistItem*>* playlistPtr) { m_playlistPtr = playlistPtr; };
	inline void setCollections(CCollection<CPlaylistItem*>* playlistPtr, CCollection<CCollectionItem*>* collectionPtr)
	{
		m_playlistPtr = playlistPtr;
		m_collectionPtr = collectionPtr;
	};

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;

	void append(QList<CPlaylistItem*> newItems);
	void append(CPlaylistItem* newItems);

	bool insertItem(CPlaylistItem* item, int pos);
	bool removeItem(int pos);

	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QString getItemAsString(int pos);
private:
	CCollection<CPlaylistItem*>* m_playlistPtr;
	CCollection<CCollectionItem*>* m_collectionPtr;


};

#endif /* CPLAYLISTMODEL_H_ */
