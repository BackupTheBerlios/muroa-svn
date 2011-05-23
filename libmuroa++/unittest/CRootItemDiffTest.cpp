/***************************************************************************
 *
 *   CRootItemDiffTest.cpp
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

#include "CRootItemDiffTest.h"
#include "CRootItem.h"
#include "CUtils.h"

#include <sstream>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( CRootItemDiffTest );

CRootItemDiffTest::CRootItemDiffTest() {
	// TODO Auto-generated constructor stub

}

CRootItemDiffTest::~CRootItemDiffTest() {
	// TODO Auto-generated destructor stub
}


void CRootItemDiffTest::setUp() {
	m_rootItem = new CRootItem;
}

void CRootItemDiffTest::tearDown() {
	delete m_rootItem;
}


void CRootItemDiffTest::patch() {
	string orig = CUtils::file2string( "unittest/testcases/CRootItemDiffTest_patch/origcollection.txt" );
	m_rootItem->deserialize(orig);

	string diff = CUtils::file2string( "unittest/testcases/CRootItemDiffTest_patch/diff.txt" );
	m_rootItem->patch(diff);

	string expected_modified = CUtils::file2string( "unittest/testcases/CRootItemDiffTest_patch/modcollection.txt" );
	CRootItem refItem;
	refItem.deserialize(expected_modified);

	CPPUNIT_ASSERT( (*m_rootItem) == refItem );
}




