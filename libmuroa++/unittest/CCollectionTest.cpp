/***************************************************************************
 *
 *   CCollectionTest.cpp
 *
 *   This file is part of libmuroa++                                   *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "CCollectionTest.h"
#include "CCategoryItem.h"
#include "CMediaItem.h"

#include <cmath>

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CCollectionTest );

CCollectionTest::CCollectionTest() {

}

CCollectionTest::~CCollectionTest() {

}

void CCollectionTest::setUp() {
	m_base = new CCategoryItem("", 0);
}

void CCollectionTest::tearDown() {
	delete m_base;
}


void CCollectionTest::construct() {
	cerr << "CCollectionTest::construct()" << endl;
	CCategoryItem *item;

	item = new CCategoryItem("stufe1", m_base);
	item = new CCategoryItem("stufe2", item);
	item = new CCategoryItem("stufe3", item);

	CMediaItem *mItem = new CMediaItem("Titel1", item);
}

void CCollectionTest::serializeMedia() {
	CMediaItem *mItem = new CMediaItem("Titel1", m_base);
	string result = m_base->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}

void CCollectionTest::serializeCategory() {
	CCategoryItem *cItem = new CCategoryItem("Category1", m_base);
	CMediaItem *mItem = new CMediaItem("Titel1", cItem);
	string result = m_base->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}


void CCollectionTest::serialize() {
	CCategoryItem *item;

	item = new CCategoryItem("stufe1", m_base);
	item = new CCategoryItem("stufe2", item);
	item = new CCategoryItem("stufe3", item);

	CMediaItem *mItem = new CMediaItem("Titel1", item);

	string result = m_base->serialize();

	stringstream ss;
	ss << mItem->getText();
	string expected = ss.str();
	CPPUNIT_ASSERT(result.compare(expected) == 0 );
}

void CCollectionTest::deserialize() {
	prepareFakeCollection(m_base, 10, 10, 10);

	string serialisation = m_base->serialize();

	cerr << serialisation << endl << endl;

	CCategoryItem* newBase = new CCategoryItem( serialisation );

	CPPUNIT_ASSERT( (*newBase) == (*m_base) );
}


void CCollectionTest::traverse() {

}

void CCollectionTest::replaceTabs() {
	CMediaItem *mItem = new CMediaItem("Tit\tel1", 0);
	mItem->setArtist("Tabula\tRasa\t");
	mItem->setAlbum("On The \t way to \tabs");
	mItem->setTitle("\tTab\tTab\t");

	string name = mItem->getName();
	CPPUNIT_ASSERT( name.compare("Tit el1") == 0 );

	string artist = mItem->getArtist();
	CPPUNIT_ASSERT( artist.compare("Tabula Rasa") == 0 );

	string album = mItem->getAlbum();
	CPPUNIT_ASSERT( album.compare("On The   way to  abs") == 0 );

	string title = mItem->getTitle();
	CPPUNIT_ASSERT( title.compare("Tab Tab") == 0 );
}




CCategoryItem* CCollectionTest::prepareFakeCollection(CCategoryItem* base, int numArtist, int numAlbum, int numTitle) {

	for(int i=0; i < numArtist; i++) {
		stringstream artist_ss;
		artist_ss << "Test Artist " << setw( floor(log10(numArtist)) ) << i;
		CCategoryItem* artist = new CCategoryItem(artist_ss.str(), base);

		for(int j=0; j < numAlbum; j++) {
			stringstream album_ss;
			album_ss << "Test Album " << setw( floor(log10(numAlbum)) ) << j;
			CCategoryItem* album = new CCategoryItem(album_ss.str(), artist);

			for(int k=0; k < numArtist; k++) {
				stringstream title_ss;
				title_ss << "Test Title " << setw( floor(log10(numTitle)) ) << k;
				CMediaItem* title = new CMediaItem(title_ss.str(), album);
				title->setFilename("/path/to/file.mp3");
				title->setArtist(artist_ss.str());
				title->setAlbum(album_ss.str());
				title->setTitle(title_ss.str());
				title->setYear(2008);
				title->setDuration( 10*k );

			}
		}
	}

	return base;
}


