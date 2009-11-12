/*
 * CCollectionModel.cpp
 *
 *  Created on: 17 Oct 2009
 *      Author: martin
 */

#include "CCollectionModel.h"
#include "CCollection.h"

CCollectionModel::CCollectionModel(QObject* parent) : QAbstractTableModel(parent)
{
}

CCollectionModel::~CCollectionModel()
{
}


int CCollectionModel::rowCount(const QModelIndex &parent) const
{
	return m_collectionPtr->size();
}

int CCollectionModel::columnCount(const QModelIndex &parent) const
{
	return m_collectionPtr->getNumColumns();
}

void CCollectionModel::append(QList<CCollectionItem> newItems)
{
    beginInsertRows(QModelIndex(), m_collectionPtr->size(), m_collectionPtr->size() + newItems.size() - 1);

	m_collectionPtr->append(newItems);

    endInsertRows();
}

void CCollectionModel::append(CCollectionItem newItem)
{
    beginInsertRows(QModelIndex(), m_collectionPtr->size(), m_collectionPtr->size());

    m_collectionPtr->append(newItem);

    endInsertRows();
}


bool CCollectionModel::insertRows(int row, int count, const QModelIndex & parent)
{

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i=0; i < count; i++) {

    }

    endInsertRows();
    return true;

}

bool CCollectionModel::removeRows(int row, int count, const QModelIndex & parent)
{

}


QVariant CCollectionModel::data(const QModelIndex &index, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	return m_collectionPtr->data(index.row(), index.column());
}

QVariant CCollectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role !=Qt::DisplayRole)
	{
		return QVariant();
	}
	if( orientation == Qt::Horizontal )
	{
		return m_collectionPtr->getTitle(section);
	}
	else
	{
		return QVariant();
	}

}
