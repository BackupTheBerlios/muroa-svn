/***************************************************************************
 *
 *   CCategoryItem.h
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

#ifndef CCATEGORYITEM_H_
#define CCATEGORYITEM_H_

#include "CItemBase.h"

#include <string>
#include <sstream>


class CCategoryItem : public CItemBase {
public:
	CCategoryItem(std::string text = std::string(), CItemBase*  parent = 0);
	virtual ~CCategoryItem();

	void addChild(CItemBase* newChild);
	std::string serialize();

	bool operator==(const CCategoryItem& other);
	inline bool operator!=(const CCategoryItem& other) { return !operator==(other);}

	static std::string getParentPath(std::string ownPath);
private:
	std::vector<CItemBase*>  m_children;
	std::istringstream m_iss;

	int m_depth;
};

#endif /* CCATEGORYITEM_H_ */
