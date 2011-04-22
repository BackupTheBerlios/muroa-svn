/***************************************************************************
 *
 *   CCategoryItem.cpp
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

#include "CCategoryItem.h"

using namespace std;

CCategoryItem::CCategoryItem(string name, CItemBase*  parent) : CItemBase(name, parent) {

}

CCategoryItem::CCategoryItem(string text): CItemBase(text) {

}


CCategoryItem::~CCategoryItem() {
	std::vector<CItemBase*>::iterator it;
	for(it = m_children.begin(); it != m_children.end(); it++ ) {
		delete *it;
	}
}

void CCategoryItem::addChild(CItemBase* newChild) {
	m_children.push_back(newChild);
}

string CCategoryItem::serialize() {
	string result;

	std::vector<CItemBase*>::iterator it;
	for(it = m_children.begin(); it != m_children.end(); it++ ) {
		result.append((*it)->serialize());
	}

	return result;
}

bool CCategoryItem::operator==(const CCategoryItem& other) {

	if(m_path != other.m_path) {
		return false;
	}

	if(m_name != other.m_name) {
		return false;
	}

	if( m_children.size() != other.m_children.size() ) {
		return false;
	}

	vector<CItemBase*>::const_iterator it = m_children.begin();
	vector<CItemBase*>::const_iterator other_it = other.m_children.begin();

	while( it != m_children.end() && other_it != other.m_children.end()) {
		if( (*it) != (*other_it)) {
			return false;
		}
		it++;
		other_it++;
	}

	return true;
}


