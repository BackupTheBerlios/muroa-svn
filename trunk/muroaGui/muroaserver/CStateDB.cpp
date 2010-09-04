/*
 * CStateDB.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: martin
 */

#include "CStateDB.h"

#include "CSession.h"

#include <iostream>
#include <sstream>

#include <QDebug>

using namespace std;

CStateDB::CStateDB(std::string dbFileName) : m_dbFileName( dbFileName ),
		                                     m_db(0),
		                                     m_selectColRevStmt(0),
		                                     m_getColItemByPosStmt(0)
{
}

CStateDB::~CStateDB() {
	close();
}


int CStateDB::open() {
	int rc = sqlite3_open(m_dbFileName.c_str(), &m_db);
	if( rc ){
	    cerr << "Can't open database: " << sqlite3_errmsg(m_db) << endl;
	    sqlite3_close(m_db);
	    m_db = 0;
    }
	else {
		createGeneralTable();
		createCollectionTable();
		createCollectionRevisionsTable();

		createPlaylistsTable();
		createNextlistsTable();

		prepareSelectColRevStmt();
		prepareGetColItemByPosStmt();
	}
}

int CStateDB::close() {
    finalizeSelectColRevStmt();
	finalizeGetColItemByPosStmt();
	if(m_db != 0) {
		sqlite3_close(m_db);
	}
}

std::string CStateDB::getValue(std::string key) {
	sqlite3_stmt *pStmt;
	const char *pzTail;
	string value;

	stringstream ss;
	ss << "SELECT value FROM general WHERE key='" << key << "'";
	string sql_stmt = ss.str();

	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	do {
		CCollectionItem* item;

		retval = sqlite3_step( pStmt );
		switch(retval) {
		case SQLITE_ROW:
		{
			const unsigned char *val = sqlite3_column_text(pStmt, 0);
			int valSize = sqlite3_column_bytes(pStmt, 0);
			value = reinterpret_cast<const char*>(val);
			break;
		}
		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db);
			break;
		}

	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
	return value;
}

void CStateDB::setValue(std::string key, std::string value) {
	sqlite3_stmt *pStmt;
	const char *pzTail;

	stringstream ss;
	ss << "INSERT OR REPLACE INTO general (key, value) VALUES "
	   << "('" << key << "','" << value << "')";
	string sql_stmt = ss.str();

	cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_step( pStmt );
	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}

void CStateDB::saveSession(CSession const * const session) {
	updateCollectionDB(session);
	updatePlaylistsTable(session);
	updateNextlistsTable(session);
}

void CStateDB::restoreSession(CSession const * session) {
	restoreCollection(session);
	restorePlaylists(session);
	restoreNextlists(session);
}



void CStateDB::createGeneralTable() {
	createTable("general", "(key TEXT UNIQUE, value TEXT)");
}

void CStateDB::createCollectionTable( ) {
	createTable("collection" , "(hash INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)");
}

void CStateDB::createCollectionRevisionsTable() {
	createTable("collectionRevs" , "(colPos INTEGER, colHash INTEGER REFERENCES collection (hash),  ColRev INTEGER)");
}

void CStateDB::createPlaylistsTable() {
	createTable("playlists","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, pl_pos INTEGER, pl_rev INTEGER, col_id INTEGER, FOREIGN KEY(col_id) REFERENCES collection(song_id))");
}

void CStateDB::createNextlistsTable() {
	createTable("nextlists","(entry_id INTEGER PRIMARY KEY AUTOINCREMENT, nl_pos INTEGER, nl_rev INTEGER, pl_id INTEGER, FOREIGN KEY(pl_id) REFERENCES playlists(entry_id))");
}


void CStateDB::updateCollectionDB( CSession const * const session, int minrev, int maxrev ) {
	if(minrev == -1) minrev = session->getMinCollectionRevision();
	if(maxrev == -1) maxrev = session->getCollectionRevision();

	for(int rev = minrev; rev < maxrev; rev++) {
		CCollection<CCollectionItem>* collection = session->getCollection(rev);

		for(int i = 0; i < collection->size(); i++) {
			CCollectionItem* item = collection->getItem(i);
			updateCollectionItem(item);
			updateCollectionRevItem(i, item->getHash(), rev );
		}
	}
}

void CStateDB::updateCollectionItem( CCollectionItem* item ) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */
	static int id = 0;
	id++;
	stringstream ss;
	// (song_id INTEGER, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	ss << "INSERT OR REPLACE INTO collection "
	   << "( file, hash, artist, album, title, duration, num_played, num_skipped, num_repeated, rating)"
	   << " VALUES "
	   << "('" << item->getFilename().toUtf8().data() << "','" << item->getHash() << "','" << item->getArtist().toUtf8().data() << "','" << item->getAlbum().toUtf8().data() << "','" << item->getTitle().toUtf8().data() << "','" << item->getLengthInSec() << "','"<< 0 <<"','" << 0 << "','"<< 0 <<"','" << 0 << "') ";
	string sql_stmt = ss.str();

	cerr << "file: " << item->getFilename().toUtf8().data() << endl;
	cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	do {
		retval = sqlite3_step( pStmt );
	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}

void CStateDB::updateCollectionRevItem( int pos, int hash, int rev ) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */
	static int id = 0;
	id++;

	int rowid = rowIDofColRevEntry(pos, hash, rev);
	if(rowid == 0) {  // this entry does not exist yet
		stringstream ss;
		// (pos INTEGER, hash INTEGER, rev INTEGER)";
		ss << "INSERT OR REPLACE INTO collectionRevs "
		   << "( colPos, colHash, colRev)"
		   << " VALUES "
		   << "('" << pos << "','" << hash << "','" << rev << "') ";
		string sql_stmt = ss.str();

		cerr << "update collectionRev SQL statement: " << sql_stmt << endl;
		int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

		if(retval != SQLITE_OK ) {
			cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
		}

		do {
			retval = sqlite3_step( pStmt );
		}while (retval == SQLITE_ROW);

		if(retval != SQLITE_DONE) {
			cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
		}

		retval = sqlite3_finalize( pStmt );
		if(retval != SQLITE_OK) {
			cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
		}
	}
}


CCollectionItem* CStateDB::getCollectionItemByHash(unsigned hash) {
	CCollectionItem* item;
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */
	stringstream ss;
	// (song_id INTEGER, file TEXT, hash INTEGER, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	ss << "SELECT * FROM collection WHERE hash='" << hash << "'";
	string sql_stmt = ss.str();

	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	do {
		retval = sqlite3_step( pStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getCollectionItemFromStmt(pStmt);
			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db);
			break;
		}

	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CCollectionItem* CStateDB::getCollectionItemByPos(int colPos, int colRev) {
	CCollectionItem* item;

	assert(m_getColItemByPosStmt != 0);

	int retval = sqlite3_bind_int(m_getColItemByPosStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_getColItemByPosStmt, 2, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to getColItemByPos statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_getColItemByPosStmt );
		switch(retval) {
		case SQLITE_ROW:
			item = getCollectionItemFromStmt(m_getColItemByPosStmt);
			num_found++;
 			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db) << endl;
			break;
		}
	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping getColItemByPos: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_getColItemByPosStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting getColItemByPos statement: " << sqlite3_errmsg(m_db);
	}
	return item;
}

CCollectionItem* CStateDB::getCollectionItemFromStmt(sqlite3_stmt *pStmt) {
	CCollectionItem* item = new CCollectionItem;
	// (hash INTEGER, file TEXT, artist TEXT, album TEXT, title TEXT, duration INTEGER, num_played INTEGER, num_skipped INTEGER, num_repeated INTEGER, rating INTEGER)";
	int numCol = sqlite3_column_count(pStmt);
	cerr << "result has " << numCol << " columns" << endl;

	int hash = sqlite3_column_int(pStmt, 0);

	const unsigned char *filename = sqlite3_column_text(pStmt, 1);
	int filenameSize = sqlite3_column_bytes(pStmt, 1);

	const unsigned char *artist = sqlite3_column_text(pStmt, 2);
	int artistSize = sqlite3_column_bytes(pStmt, 3);

	const unsigned char *album = sqlite3_column_text(pStmt, 3);
	int albumSize = sqlite3_column_bytes(pStmt, 4);

	const unsigned char *title = sqlite3_column_text(pStmt, 4);
	int titleSize = sqlite3_column_bytes(pStmt, 5);

	cerr << "SELECT: " << filename << " " << artist << " " << album << " " << title << endl;

	return item;
}


/** \brief  Create a database table if it does not yet exist
 *
 *
 * \param   name   Name of the table to create
 * \param   schema Table schema. e.g. "(rev_id INTEGER, rev INTEGER)"
 * \exception  todexception on failure
 */
void CStateDB::createTable(std::string name, std::string schema) {
	sqlite3_stmt *pStmt;    /* OUT: Statement handle */
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */

	stringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS " << name << " " << schema;
	string sql_stmt = ss.str();

	cerr << "SQL statement: " << sql_stmt << endl;
	int retval = sqlite3_prepare_v2(m_db, sql_stmt.c_str(), sql_stmt.size(), &pStmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_step( pStmt );

	if(retval != SQLITE_DONE) {
		cerr << "Error while creating table: " << sqlite3_errmsg(m_db);
	}

	retval = sqlite3_finalize( pStmt );
	if(retval != SQLITE_OK) {
		cerr << "Error finalizing create table: " << sqlite3_errmsg(m_db);
	}
}

int CStateDB::rowIDofColRevEntry(int colPos, int colHash, int colRev) {
	int retval = sqlite3_bind_int(m_selectColRevStmt, 1, colPos);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colPos' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectColRevStmt, 2, colHash);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colHash' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}
	retval = sqlite3_bind_int(m_selectColRevStmt, 3, colRev);
	if(retval != SQLITE_OK) {
		cerr << "Error binding value 'colRev' to selectColRev statement: " << sqlite3_errmsg(m_db) << endl;
	}

	int rowid = 0;
	int num_found = 0;
	do {
		retval = sqlite3_step( m_selectColRevStmt );
		switch(retval) {
		case SQLITE_ROW:
			rowid =	sqlite3_column_int(m_selectColRevStmt, 0);
			num_found++;
 			break;

		case SQLITE_DONE:
			// no more rows match search
			break;

		default:
			cerr << "Error during step command: " << sqlite3_errmsg(m_db) << endl;
			break;
		}

	}while (retval == SQLITE_ROW);

	if(retval != SQLITE_DONE) {
		cerr << "Error stepping table columnRevs: " << sqlite3_errmsg(m_db);
	}
	retval = sqlite3_reset(m_selectColRevStmt);
	if(retval != SQLITE_OK) {
		cerr << "Error resetting statement: " << sqlite3_errmsg(m_db);
	}
	return rowid;
}

void CStateDB::updatePlaylistsTable(CSession const * const session) {

}

void CStateDB::updateNextlistsTable(CSession const * const session) {

}

void CStateDB::restoreCollection(CSession const * session) {

}

void CStateDB::restorePlaylists(CSession const * session) {

}

void CStateDB::restoreNextlists(CSession const * session) {

}


void CStateDB::prepareSelectColRevStmt() {
	prepareStmt(&m_selectColRevStmt, "SELECT rowid FROM collectionRevs WHERE colPos=? AND colHash=? AND colRev=?");
}

void CStateDB::finalizeSelectColRevStmt() {
	finalizeStmt( &m_selectColRevStmt );
}


void CStateDB::prepareGetColItemByPosStmt() {
	prepareStmt(&m_getColItemByPosStmt, "SELECT * FROM collection INNER JOIN collectionRevs ON collection.hash=collectionRevs.colHash WHERE collectionRevs.colPos=? AND collectionRevs.colRev=?");
}

void CStateDB::finalizeGetColItemByPosStmt() {
	finalizeStmt( &m_getColItemByPosStmt );
}

void CStateDB::prepareStmt(sqlite3_stmt** stmt, std::string sqlStmt) {
	const char *pzTail;      /* OUT: Pointer to unused portion of zSql */

	int retval = sqlite3_prepare_v2(m_db, sqlStmt.c_str(), sqlStmt.size(), stmt, &pzTail);

	if(retval != SQLITE_OK ) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(m_db) << endl;
	}
}

void CStateDB::finalizeStmt(sqlite3_stmt** stmt) {
	int retval;
	if( *stmt ) {
		retval = sqlite3_finalize( *stmt );
		if(retval != SQLITE_OK) {
			cerr << "Error finalizing prepared statement: " << sqlite3_errmsg(m_db) << endl;
		}
		*stmt = 0;
	}
}

