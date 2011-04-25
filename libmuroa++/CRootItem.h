/*
 * CRootItem.h
 *
 *  Created on: 22 Apr 2011
 *      Author: martin
 */

#ifndef CROOTITEM_H_
#define CROOTITEM_H_

#include <string>
#include <map>

class CItemBase;
class CCategoryItem;
class CMediaItem;

class CRootItem {
public:
	CRootItem();
	virtual ~CRootItem();

	CItemBase* getItemPtr(std::string path) {
		return m_map[path];
	}

	inline void setItemPtr(std::string path, CItemBase* itemPtr) {
		m_map.insert(std::make_pair(path, itemPtr));
	}

	CCategoryItem* addCategory(std::string name, CCategoryItem* parent = 0);
	CMediaItem* addMediaItem(std::string name, CCategoryItem* parent = 0);

	std::string serialize();
	void deserialize(std::string text);

	bool operator==(const CRootItem& other);


private:
	std::map<std::string, CItemBase*> m_map;
	CItemBase* m_base;
};

#endif /* CROOTITEM_H_ */
