/*
 * CSession.cpp
 *
 *  Created on: 6 Nov 2009
 *      Author: martin
 */

#include "CSession.h"
#include "CDiff.h"
#include "CConnection.h"


CSession::CSession() : m_latestCollectionRevision(0),
                       m_latestPlaylistRevision(0),
                       m_latestNextlistRevision(0),
                       m_minCollectionRevision(0),
                       m_minPlaylistRevision(0),
                       m_minNextlistRevision(0),
                       m_playlistPos(0),
                       m_stateDB("state.db")
{
	// all thee collection have an empty revision 0!
	m_collectionRevisions[m_latestCollectionRevision] = new CCollection<CCollectionItem>(m_latestCollectionRevision);
	m_playlistRevisions[m_latestPlaylistRevision] = new CCollection<CPlaylistItem>(m_latestPlaylistRevision);
	m_nextlistRevisions[m_latestNextlistRevision] = new CCollection<CPlaylistItem>(m_latestNextlistRevision);

	connect(&m_stream, SIGNAL(finished()), this, SLOT(next()));
	connect(&m_stream, SIGNAL(progress(int, int)), this, SLOT(progress(int, int)));
}

CSession::~CSession() {
	disconnect(&m_stream, SIGNAL(progress(int, int)), this, SLOT(progress(int, int)));

}

CCollection<CCollectionItem>* CSession::getCollection(int revision) const
{
	if(m_collectionRevisions.contains(revision))
	{
		return m_collectionRevisions.value(revision);
	}
	else
	{
		return 0;
	}
}

CCollection<CPlaylistItem>* CSession::getPlaylist(int revision) const
{
	if(m_playlistRevisions.contains(revision))
	{
		return m_playlistRevisions.value(revision);
	}
	else
	{
		return 0;
	}
}

CCollection<CPlaylistItem>* CSession::getNextlist(int revision) const
{
	if(m_nextlistRevisions.contains(revision))
	{
		return m_nextlistRevisions.value(revision);
	}
	else
	{
		return 0;
	}
}

const QString CSession::getCollectionDiff(int fromRevision, int toRevision)
{
	if(toRevision == -1)
	{
		toRevision = m_latestCollectionRevision;
	}

	if(fromRevision == -1
	   || ! m_collectionRevisions.contains(fromRevision)
	   || ! m_collectionRevisions.contains(toRevision))
	{
		return QString();
	}

	CDiff differ;

	QString knownRev = m_collectionRevisions.value(fromRevision)->getText();
	QString targetRev = m_collectionRevisions.value(toRevision)->getText();
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}


const QString CSession::getPlaylistDiff(int fromRevision, int toRevision)
{
	if(toRevision == -1)
	{
		toRevision = m_latestPlaylistRevision;
	}

	if(fromRevision == -1
	   || ! m_playlistRevisions.contains(fromRevision)
	   || ! m_playlistRevisions.contains(toRevision))
	{
		return QString();
	}

	CDiff differ;

	QString knownRev = m_playlistRevisions.value(fromRevision)->getText();
	QString targetRev = m_playlistRevisions.value(toRevision)->getText();
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}

const QString CSession::getNextlistDiff(int fromRevision, int toRevision)
{
	if(toRevision == -1)
	{
		toRevision = m_latestNextlistRevision;
	}

	if(fromRevision == -1
	   || ! m_nextlistRevisions.contains(fromRevision)
	   || ! m_nextlistRevisions.contains(toRevision))
	{
		return QString();
	}

	CDiff differ;

	QString knownRev = m_nextlistRevisions.value(fromRevision)->getText();
	QString targetRev = m_nextlistRevisions.value(toRevision)->getText();
	QString diffString = differ.diff(knownRev, targetRev);

	return diffString;
}

void CSession::addCollectionRev(CCollection<CCollectionItem>* collection) {
	m_latestCollectionRevision++;
	if(collection->getRevision() == -1) {
		collection->setRevision(m_latestCollectionRevision);
	}
	m_collectionRevisions[m_latestCollectionRevision] = collection;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendCollection(collection);
	}

}

void CSession::addCollectionRev(QString collection)
{
	CCollection<CCollectionItem>* newCollection = new CCollection<CCollectionItem>();
	newCollection->setText(collection, m_latestCollectionRevision + 1);

	qDebug() << newCollection->getText();

	addCollectionRev(newCollection);
}

void CSession::addPlaylistRev(CCollection<CPlaylistItem>* playlist) {
	m_latestPlaylistRevision++;
	if(playlist->getRevision() == -1) {
		playlist->setRevision(m_latestPlaylistRevision);
	}
	m_playlistRevisions[m_latestPlaylistRevision] = playlist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylist(playlist);
	}
}


void CSession::addPlaylistRev(QString playlist)
{
	m_latestPlaylistRevision++;
	CCollection<CPlaylistItem>* newPlaylist = new CCollection<CPlaylistItem>();
	newPlaylist->setText(playlist, m_latestPlaylistRevision);

	qDebug() << newPlaylist->getText();

	m_playlistRevisions[m_latestPlaylistRevision] = newPlaylist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylist(newPlaylist);
	}
}

void CSession::addNextlistRev(CCollection<CPlaylistItem>* nextlist) {
	m_latestNextlistRevision++;
	if(nextlist->getRevision() == -1) {
		nextlist->setRevision(m_latestNextlistRevision);
	}
	m_nextlistRevisions[m_latestNextlistRevision] = nextlist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylist(nextlist);
	}
}

void CSession::addNextlistRev(QString nextlist)
{
	m_latestNextlistRevision++;
	CCollection<CPlaylistItem>* newNextlist = new CCollection<CPlaylistItem>();
	newNextlist->setText(nextlist, m_latestNextlistRevision);

	qDebug() << newNextlist->getText();

	m_nextlistRevisions[m_latestNextlistRevision] = newNextlist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendNextlist(newNextlist);
	}
}

int CSession::addCollectionRevFromDiff(QString* collectionDiff, int diffFromRev) throw(InvalidMsgException)
{
	if( diffFromRev != m_latestCollectionRevision ) {
		throw InvalidMsgException("Base revision of diff is not latest revision of collection.");
	}
	CCollection<CCollectionItem>* newCollection = new CCollection<CCollectionItem>( *(getCollection(m_latestCollectionRevision)) );

	newCollection->patch(collectionDiff, m_latestCollectionRevision + 1);
	// if patch fails, an exception will be thrown that will be caught by the caller of this method.
	// Incrementing m_latestCollectionRevision after call to patch assures, that m_latestCollectionRevision
	// will still have the correct old value in case of exception.

	m_latestCollectionRevision++;
	qDebug() << newCollection->getText();
	m_collectionRevisions[m_latestCollectionRevision] = newCollection;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendCollectionDiff(m_latestCollectionRevision, diffFromRev, *collectionDiff);
	}
}

int CSession::addPlaylistRevFromDiff(QString* playlistDiff, int diffFromRev) throw(InvalidMsgException)
{
	qDebug() << QString("CSession::addPlaylistRevFromDiff %1 %2").arg(*playlistDiff).arg(diffFromRev);
	if( diffFromRev != m_latestPlaylistRevision ) {
		throw InvalidMsgException("Base revision of diff is not latest revision of playlist.");
	}
	CCollection<CPlaylistItem>* newPlaylist = new CCollection<CPlaylistItem>( *(getPlaylist(m_latestPlaylistRevision)) );
	newPlaylist->patch(playlistDiff, m_latestPlaylistRevision + 1);

	// increase m_latestPlaylistRevision after call to patch because patch may throw an exception caught
	// by the caller of this method. In that case m_latestPlaylistRevision must keept its old value.
	m_latestPlaylistRevision++;
	qDebug() << newPlaylist->getText();

	m_playlistRevisions[m_latestPlaylistRevision] = newPlaylist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendPlaylistDiff(m_latestPlaylistRevision, diffFromRev, *playlistDiff);
	}
}

int CSession::addNextlistRevFromDiff(QString* nextlistDiff, int diffFromRev) throw(InvalidMsgException)
{
	qDebug() << QString("CSession::addNextlistRevFromDiff %1 %2").arg(*nextlistDiff).arg(diffFromRev);
	if( diffFromRev != m_latestNextlistRevision ) {
		throw InvalidMsgException("Base revision of diff is not latest revision of nextlist.");
	}
	CCollection<CPlaylistItem>* newNextlist = new CCollection<CPlaylistItem>( *(getNextlist(m_latestNextlistRevision)) );
	newNextlist->patch(nextlistDiff, m_latestNextlistRevision + 1);
	// increase m_latestPlaylistRevision after call to patch because patch may throw an exception caught
	// by the caller of this method. In that case m_latestPlaylistRevision must keept its old value.
	m_latestNextlistRevision++;
	qDebug() << newNextlist->getText();

	m_nextlistRevisions[m_latestNextlistRevision] = newNextlist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendNextlistDiff(m_latestNextlistRevision, diffFromRev, *nextlistDiff);
	}
}


int CSession::addNextlistRevFromNextCmd() {
	qDebug() << QString("CSession::addNextlistRevFromNextCmd");
	CCollection<CPlaylistItem>* newNextlist = new CCollection<CPlaylistItem>( *(getNextlist(m_latestNextlistRevision)) );
	m_latestNextlistRevision++;
	qDebug() << newNextlist->getText();

	// handle next cmd here:
	//  1) if there are any items in nextlist, remove first one. It has just been played.
	//  2) if nextlist is empty now, append the playlist item from m_playlistpos to nextlist.
	if( newNextlist->size() > 0 ) {
		CPlaylistItem* item = newNextlist->takeAt(0);
		delete item;
	}

	if( newNextlist->size() < 1 ) {
		// append item from playlist
		CPlaylistItem * item = getPlaylist()->getItem( m_playlistPos );
		m_playlistPos++;
		newNextlist->insert(item, -1);
	}

	qDebug() << newNextlist->getText();

	m_nextlistRevisions[m_latestNextlistRevision] = newNextlist;

	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendNextlist(newNextlist);
	}

}

int CSession::addNextlistRevFromPrevCmd() {

}

void CSession::setMinCollectionRevision(int rev) throw() {
	if(m_collectionRevisions.size() > 1) {
		throw(CApiMisuseException("Trying to set m_minCollectionRevision with non-empty collection revisions."));
	}
	m_minCollectionRevision = rev;
	m_latestCollectionRevision = rev - 1;
}

void CSession::setMinPlaylistRevision(int rev) throw() {
	m_minPlaylistRevision = rev;
	m_latestPlaylistRevision = rev - 1;
}

void CSession::setMinNextlistRevision(int rev) throw() {
	m_minNextlistRevision = rev;
	m_latestNextlistRevision = rev - 1;
}

void CSession::addConnection(CConnection* connection)
{
	if(!m_connections.contains(connection))
	{
		// avoid duplicates
		m_connections.append(connection);
		connection->setSessionPtr(this);
	}
}

void CSession::connectionClosed(CConnection* conn)
{
	m_connections.removeAll(conn);
	delete conn;
}


void CSession::play()
{
	setSong();
	m_stream.play();
}

void CSession::stop()
{
	m_stream.stop();
}

void CSession::next()
{
	addNextlistRevFromNextCmd();
	setSong();
}

void CSession::setSong() {
	if( getNextlist()->size() == 0 ) {
		addNextlistRevFromNextCmd();
	}
	CPlaylistItem*  plItem = getNextlist()->getItem(0);
	CCollectionItem* item = getCollection()->getByHash( plItem->getCollectionHash() );
	if(item != 0)
	{
		m_stream.setSong( item );
	}
}


void CSession::prev()
{
	addNextlistRevFromPrevCmd();
}


void CSession::progress(int done, int total)
{
	for(int i=0; i < m_connections.size(); i++)
	{
		m_connections.at(i)->sendProgress(done, total);
	}
}

void CSession::scanProgress(int donw, int total) {
	// send only to admin clients

}



void CSession::saveState() {

}

void CSession::restoreState() {

}

