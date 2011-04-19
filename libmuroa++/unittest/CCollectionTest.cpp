/***************************************************************************
 *
 *   CCollectionTest.cpp
 *
 *   This file is part of libmuroa++                                  *
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

#include <iostream>

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


void CCollectionTest::serialize() {

}


void CCollectionTest::traverse() {

}



void CCollectionTest::construct() {
	cerr << "CCollectionTest::construct()" << endl;
	CCategoryItem *item;

	item = new CCategoryItem("stufe1", m_base);
	item = new CCategoryItem("stufe2", item);
	item = new CCategoryItem("stufe3", item);

	CMediaItem *mItem = new CMediaItem("Titel1", item);

}




void CCollectionTest::deserialize() {

}


