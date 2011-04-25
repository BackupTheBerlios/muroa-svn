/***************************************************************************
 *
 *   CItemBase.cpp
 *
 *   This file is part of libmuroa++                                  *
 *   Copyright (C) 2011 by martin                                      *
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

#include "CItemBase.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

CItemBase::CItemBase(std::string text, CItemBase*  parent, bool deserialize) :
           m_parent(parent)
{
	if(deserialize) {
		int pos = text.find('\t', 0);
		m_name = text.substr(0, pos);
	}
	else {
		m_name = text;
		replaceTabs(m_name);
	}
	if(m_parent) {
		m_path = m_parent->getPath();
		m_path.append("\t");
	}
	m_path.append(m_name);
	if(m_parent) {
		m_parent->addChild(this);
	}
}

CItemBase::CItemBase(std::string text ) {
	int pos = text.find('\t', 0);
	m_path = text.substr(0, pos);
}


CItemBase::~CItemBase() {
	// traversing the tree for cleanup is done in CCategoryItem as CMediaItem may not have any children.
}


void CItemBase::replaceTabs(string& str) {
	std::replace( str.begin(), str.end(), '\t', ' ' );
	trim(str);
}

