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
#include <iostream>

#include <stdexcept>

class CItemBase;
class CCategoryItem;
class CMediaItem;

class CRootItem {
public:
	CRootItem();
	virtual ~CRootItem();

	CItemBase* getItemPtr(std::string path) {
    	std::map<std::string, CItemBase*>::iterator it;
    	it = m_map.find(path);
    	if(it == m_map.end()) {
    		return 0;
    	}
    	else {
    		return it->second;
    	}
   	}

	inline void setItemPtr(std::string path, CItemBase* itemPtr) {
		std::pair<std::map<std::string, CItemBase*>::iterator,bool> ret;
		ret = m_map.insert(std::pair<std::string,CItemBase*>(path, itemPtr));
		if (ret.second==false)
		{
		    std::cout << "element 'z' already existed";
		    std::cout << " with a value of " << ret.first->second << std::endl;
		}
	}

	CCategoryItem* addCategory(std::string name, CCategoryItem* parent = 0);

	CMediaItem* addMediaItem(CCategoryItem* parent = 0);
	CMediaItem* addMediaItem(std::string textWoPath, CCategoryItem* parent);
	CMediaItem* addMediaItem(std::string text);

	std::string serialize();
	void deserialize(std::string text);

	std::string diff(CRootItem& other);
	void patch(std::string diff) throw(std::invalid_argument);

	bool operator==(const CRootItem& other);


private:
	std::map<std::string, CItemBase*> m_map;
	CCategoryItem* m_base;

	CCategoryItem* mkPath(std::string path);
	long str2long(std::string str) throw(std::invalid_argument);
};

#endif /* CROOTITEM_H_ */
