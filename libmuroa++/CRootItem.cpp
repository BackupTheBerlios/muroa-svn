/*
 * CRootItem.cpp
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#include "CRootItem.h"

#include "CCategoryItem.h"
#include "CMediaItem.h"
#include "CDiff.h"

#include <iostream>
#include <sstream>

#include <regex>

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
	if(pathPos == string::npos) {
		return 0;
	}
	string path = text.substr(0, pathPos);

	CItemBase* parent = getItemPtr(path);
	if(parent == 0) {
		parent = mkPath(path);
	}

	string mItemText = text.substr(pathPos, text.size() - pathPos);
	CMediaItem* newItem = new CMediaItem(mItemText, parent);
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

	}
}



std::string CRootItem::serialize() {
	return m_base->serialize();
}



string CRootItem::diff(CRootItem& other) {
	CDiff differ;
	string diff = differ.diff( serialize(), other.serialize() );

	return diff;
}

void CRootItem::patch(std::string diff) throw(std::invalid_argument) {
	istringstream iss(diff);

    std::regex rx("^@@ -(\\d+),(\\d+)\\s+\\+(\\d+),(\\d+)\\s*@@$");
    int oldStart(0);
	int oldLen(0);
	int newStart(0);
	int newLen(0);

	string line;
	while(!iss.eof()) {
		getline(iss, line);
		if(iss.bad()) {
			cerr << "CRootItem::patch: Error reading lines." << endl;
		}

		if( line.find("@@") == 0 ) {
			// diff chunk header
		    std::cmatch res;
		    std::regex_search(line.c_str(), res, rx);

		    string oldStartStr = res[1];
			string oldLenStr = res[2];
			string newStartStr = res[3];
			string newLenStr = res[4];

			oldStart = str2long( oldStartStr );
			oldLen = str2long( oldLenStr );
			newStart = str2long( newStartStr );
			newLen = str2long( newLenStr );


		}
		else {



		}
	}


}




bool CRootItem::operator==(const CRootItem& other) {
	return (*m_base == *(other.m_base));
}


CCategoryItem* CRootItem::mkPath(string path) {
	size_t lpos = 1;
	CItemBase* parent = m_base;

	while( lpos < path.size() ) {
		size_t rpos = path.find('/', lpos);
		if(rpos == string::npos) {
			if( lpos != path.size() ) {
				rpos = path.size();
			}
			else {
				break;
			}
		}


		string catPath = path.substr(0, rpos);
		string catName = path.substr(lpos , rpos - lpos);

		CItemBase* cItem = getItemPtr(catPath);
		if(cItem == 0) {
			cItem = new CCategoryItem( catName, parent);
			setItemPtr(catPath, cItem);
		}
		parent = cItem;
		lpos = rpos + 1;
	}

	return static_cast<CCategoryItem*>(parent);
}


long CRootItem::str2long(std::string str) throw(std::invalid_argument) {
	errno = 0;
	char* endptr;
	long iVal = strtol( str.c_str(), &endptr, 10);
	if (errno != 0 || endptr != '\0' ) {
		throw invalid_argument("convert string to int");
	}
	return iVal;
}
