/*
 * CCollectionItem.h
 *
 *  Created on: 16 Oct 2009
 *      Author: martin
 */

#ifndef CCOLLECTIONITEM_H_
#define CCOLLECTIONITEM_H_

#include <QString>
#include <QStringRef>
#include <QHash>

#include "CItemBase.h"

class CCollectionItem :public CItemBase {
public:
	CCollectionItem();
	CCollectionItem(QString text);
	virtual ~CCollectionItem();

//  void setText(QString text);

//	inline QString getText() const { return m_text; };

    inline QString getAlbum() const { return m_album; }
    inline QString getArtist() const { return m_artist; }
    inline QString getFilename() const { return m_filename; }
    inline int getDuration() const { return m_duration_in_s; }
    inline QString getTitle() const { return m_title; }
    inline int getYear() const  { return m_year; }

    void setAlbum(QString album);
    void setAlbum(std::string album);

    void setArtist(QString artist);
    void setArtist(std::string artist);
    void setFilename(QString filename);
    void setFilename(std::string filename);
    void setDuration(int duration);
    void setTitle(QString title);
    void setTitle(std::string title);
    void setYear(int year);

private:
    void assembleText();

//    QString m_text;

	QString m_filename;
	QString m_artist;
	QString m_album;
	QString m_title;
	int m_year;
	int m_duration_in_s;
};

#endif /* CCOLLECTIONITEM_H_ */
