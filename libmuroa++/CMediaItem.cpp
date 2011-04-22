/*
 * CMediaItem.cpp
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#include "CMediaItem.h"

#include <sstream>
#include <functional>
#include <stdexcept>
#include <algorithm>

#include <iostream>

using namespace std;

CMediaItem::CMediaItem(string name, CItemBase*  parent) : CItemBase(name, parent) {
	rehash();
}

CMediaItem::CMediaItem(std::string text) : CItemBase(text) {

	// first section is handled by CItemBase
	size_t lpos, rpos;

	lpos = m_path.size() + 1;

	rpos = m_text.find('\t', lpos);
	m_filename = text.substr(lpos, rpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_artist = text.substr(lpos, rpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_album = text.substr(lpos, rpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	m_title = text.substr(lpos, rpos);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string yearStr = text.substr(lpos, rpos);
	m_year = strtol(yearStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	rpos = m_text.find('\t', lpos);
	string durationStr = text.substr(lpos, rpos);
	m_duration_in_s = strtol(durationStr.c_str(), NULL, 10);
	lpos = rpos + 1;

	string hashStr = text.substr(lpos);
	m_hash = strtol(hashStr.c_str(), NULL, 10);
}

CMediaItem::~CMediaItem() {
}

void CMediaItem::setAlbum(string album)
{
	replaceTabs(album);
	m_album = album;
	rehash();
}

void CMediaItem::setArtist(string artist)
{
	replaceTabs(artist);
	m_artist = artist;
	rehash();
}

void CMediaItem::setFilename(string filename)
{
	replaceTabs(filename);
	m_filename = filename;
	rehash();
}

void CMediaItem::setDuration(int duration)
{
	m_duration_in_s = duration;
	rehash();
}

void CMediaItem::setTitle(string title)
{
	replaceTabs(title);
	m_title = title;
	rehash();
}

void CMediaItem::setYear(int year)
{
	m_year = year;
	rehash();
}


void CMediaItem::rehash() {
	stringstream ss;

	ss << m_path << "\t" << m_filename << "\t" << m_artist << "\t" << m_album << "\t" << m_title << "\t" << m_year << "\t" << m_duration_in_s;

	m_hash = hash<string>()( ss.str() );
	ss << "\t" << m_hash << endl;

	m_text = ss.str();
}

bool CMediaItem::operator==(const CMediaItem& other) {
	bool rc = true;

	if( m_filename.compare( other.m_filename ) != 0 ) {
		rc = false;
	}

	if( m_artist.compare( other.m_artist ) != 0 ) {
		rc = false;
	}

	if( m_album.compare( other.m_album ) != 0 ) {
		rc = false;
	}

	if( m_title.compare( other.m_title ) != 0 ) {
		rc = false;
	}

	if( m_year != other.m_year ) {
		rc = false;
	}

	if( m_duration_in_s != other.m_duration_in_s ) {
		rc = false;
	}

	if( m_hash != other.m_hash ) {
		rc = false;
	}

	if( m_text.compare( other.m_text ) != 0 ) {
		rc = false;
	}

	return rc;
}

void CMediaItem::addChild(CItemBase* newChild) {
	throw std::runtime_error( "CMediaItem may not have children!");
}

string CMediaItem::serialize() {
	return m_text;
}