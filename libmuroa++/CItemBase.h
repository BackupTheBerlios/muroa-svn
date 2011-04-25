/***************************************************************************
 *                                                                         *
 *   CItemBase.h                                                           *
 *                                                                         *
 *   This file is part of libmuroa++                                       *
 *   Copyright (C) 2011 by Martin Runge <martin.runge@web.de>              *
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

#ifndef CITEMBASE_H_
#define CITEMBASE_H_

#include <string>
#include <vector>

class CItemBase {
public:
	CItemBase(std::string name, CItemBase*  parent, bool deserialize);
	virtual ~CItemBase();

protected:
	CItemBase(std::string text );

public:
	inline std::string getName() { return m_name; };
	inline std::string getPath() { return m_path; };

	inline CItemBase* getParent() { return m_parent; };
	virtual void addChild(CItemBase* newChild) = 0;

	inline std::string getText() { return m_text; };

	virtual std::string serialize() = 0;

protected:
	std::string m_name;
	std::string m_path;

	CItemBase*  m_parent;

	std::string m_text;
	void replaceTabs(std::string& str);

};

#endif /* CITEMBASE_H_ */
