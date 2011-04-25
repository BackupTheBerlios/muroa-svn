/*
 * CRootItem.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#include "CRootItem.h"

#include "CCategoryItem.h"
#include "CMediaItem.h"

#include <iostream>
#include <sstream>

#include <cassert>
#include <cstring>

using namespace std;

CRootItem::CRootItem() {
	m_base = new CCategoryItem("", 0);
	setItemPtr(m_base->getName(), m_base);
}

CRootItem::~CRootItem() {
	delete m_base;
}

CCategoryItem* CRootItem::addCategory(string name, CCategoryItem* parent) {
	CCategoryItem* newItem;
	if(parent == 0) {
		newItem = new CCategoryItem(name, m_base);
	}
	else {
		newItem = new CCategoryItem(name, parent);
	}
	return newItem;
}

CMediaItem* CRootItem::addMediaItem(CCategoryItem* parent) {
	if (parent == 0)  {
		parent = m_base;
	}
	CMediaItem* newItem = new CMediaItem(parent);
	return newItem;
}

CMediaItem* CRootItem::addMediaItem(string textWoPath, CCategoryItem* parent) {

	CMediaItem* newItem = new CMediaItem(textWoPath, parent);
	return newItem;
}


CMediaItem* CRootItem::addMediaItem(string text) {

	size_t pathPos = text.find('\t');
	string path = text.substr(0, pathPos);

	CItemBase* parent = getItemPtr(path);
	if(parent == 0) {
		parent = mkPath(path);
	}


	CMediaItem* newItem = new CMediaItem(text, parent);
	return newItem;
}


void CRootItem::deserialize(std::string text) {
	istringstream iss(text);
	char cline[4096];

	CItemBase* currentCategory = 0;

	while(!iss.eof()) {
		iss.getline(cline, 4096);
		if(iss.bad()) {
			cerr << "CRootItem::deserialize: Error reading lines." << endl;
		}

		CMediaItem* mItem = addMediaItem(cline);

		continue;

		if( strlen(cline) < 3)  continue;
		// assert( cline[0] != 0 && cline[1] != 0 && cline[2] != 0 );

		// char cat =  cline[0];
		// string line(cline);  // skip category letter and tab
//		switch(cat) {
//		case 'c': {
//			string name;
//			size_t rpos = line.rfind('\t');
//			if(rpos != string::npos) {
//				name = line.substr(rpos, line.size());
//			}
//			else {
//				name = line;
//			}
//			string newItemParentsName = CCategoryItem::getParentPath(line);
//			CItemBase* newItemsPrarent = getItemPtr(newItemParentsName);
//			currentCategory = newItemsPrarent;
//
//			if(newItemsPrarent != 0) {
//				CCategoryItem* newitem = new CCategoryItem(name, newItemsPrarent);
//				setItemPtr(line, newitem);
//			}
//			break;
//		}
//		case 'm': {
//			CMediaItem* mItem = new CMediaItem(line, currentCategory);
//
//			break;
//		}
//		default:
//			break;
//		}
	}
}



std::string CRootItem::serialize() {
	return m_base->serialize();
}

bool CRootItem::operator==(const CRootItem& other) {
	return (m_base == other.m_base);
}


CCategoryItem* CRootItem::mkPath(string path) {
	size_t lpos = 1;
	CItemBase* parent = m_base;

	while( lpos < path.size() ) {
		size_t rpos = path.find('/', lpos);
		if(rpos == string::npos) {
			break;
		}

		string catPath = path.substr(0, rpos);
		string catName = path.substr(lpos , rpos - lpos);

		CItemBase* cItem = getItemPtr(catPath);
		if(cItem == 0) {
			cItem = new CCategoryItem( catName, parent);
		}
		parent = cItem;
		lpos = rpos + 1;
	}

	return static_cast<CCategoryItem*>(parent);
}
