/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef FLASH_MOVIE_MANAGER_H
#define FLASH_MOVIE_MANAGER_H

#include "common/Singleton.h"

// OIS
#include "Ogre3d/include/ois/OISMouse.h"

// Forward declarations
namespace Hikari 
{
	class HikariManager;
}

/**
 * Manages Hikari to load flash (swf) movies.
 */ 
class FlashMovieManager: public Cing::SingletonStatic< FlashMovieManager >, public OIS::MouseListener
{
public:
	// Singleton requirements
	friend class Cing::SingletonStatic< FlashMovieManager >;

	virtual ~FlashMovieManager() { end(); }


	// Init/update/end
	void init	();
	void update	();
	void end	();

	// Input events
	bool mouseMoved			( const OIS::MouseEvent& event );
	bool mousePressed		( const OIS::MouseEvent& event, OIS::MouseButtonID id  );
	bool mouseReleased		( const OIS::MouseEvent& event, OIS::MouseButtonID id  );

	// Get Methods
	Hikari::HikariManager* getHikariManager() { return m_hikariMgr; }

private:
	// Private to ensure singleton
	FlashMovieManager(): m_hikariMgr(NULL) { };


	// Attributes
	Hikari::HikariManager*	m_hikariMgr;
};

#endif