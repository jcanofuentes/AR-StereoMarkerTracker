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

#include "FlashMovieManager.h"

#include "../Hikari/include/Hikari.h"

#include "framework/userAppGlobals.h"
#include "common/eString.h"
#include "input/InputManager.h"


void FlashMovieManager::init()
{
	// Create hikari manager
	m_hikariMgr = new Hikari::HikariManager( "..\\data\\Flash" );

	// Register as mouse input event listener
	Cing::InputManager::getSingleton().getMouse().addListener( this );
}

void FlashMovieManager::update()
{
	// update the hikari manager to update all swf movies loaded
	m_hikariMgr->update();
}

void FlashMovieManager::end()
{
	// Release hikari manager
	if ( m_hikariMgr )
	{
		delete m_hikariMgr;
		m_hikariMgr = NULL;
	}
}


bool FlashMovieManager::mouseMoved(const OIS::MouseEvent &arg)
{
	return m_hikariMgr->injectMouseMove(arg.state.X.abs, arg.state.Y.abs);
}

bool FlashMovieManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return m_hikariMgr->injectMouseDown(id);
}

bool FlashMovieManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return m_hikariMgr->injectMouseUp(id);
}