/***************************************************************************
 *
 *   CRootItemDiffTest.h
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

#ifndef CROOTITEMDIFFTEST_H_
#define CROOTITEMDIFFTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "CDiff.h"

#include <string>

class CCategoryItem;
class CRootItem;

class CRootItemDiffTest : public CppUnit::TestFixture {
	  CPPUNIT_TEST_SUITE( CRootItemDiffTest );
	  CPPUNIT_TEST( patch );
	  CPPUNIT_TEST_SUITE_END();

public:
	CRootItemDiffTest();
	virtual ~CRootItemDiffTest();

	void setUp();
    void tearDown();

	void patch();

private:
	CRootItem* m_rootItem;

};

#endif /* CROOTITEMDIFFTEST_H_ */
