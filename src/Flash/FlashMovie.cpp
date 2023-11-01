/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "graphics/GraphicsManager.h"
#include "framework/UserAppGlobals.h"


#include "FlashMovie.h"
#include "FlashMovieManager.h"



// Loads a flash movie (swf)
void FlashMovie::load( const Cing::String& flashFile, int width, int height, int xPos, int yPos )
{
	// If its already loaded do nothing...
	if ( isLoaded() )
		return;

	// Get the hikari manager
	FlashMovieManager& manager = FlashMovieManager::getSingleton();
	Hikari::HikariManager* hikariMgr = FlashMovieManager::getSingleton().getHikariManager();

	// Get main viewport from graphics manager
	Ogre::Viewport* viewport = Cing::GraphicsManager::getSingleton().getMainWindow().getViewport();

	static int count = 0;
	count++;


	// Create the flash control
	m_flashControl = hikariMgr->createFlashOverlay( flashFile + Cing::intToString(count), viewport, width, height, Hikari::Position(xPos, yPos), 0);
	m_flashControl->load( flashFile );
	m_flashControl->setTransparent(true, true);

	// Bind functions to be called from flash movies
	registerCFunction("closeMovie", Hikari::FlashDelegate(this, &FlashMovie::closeMovie));

	m_loaded = true;
}

void FlashMovie::end()
{
	// TODO: check why it crashes sometimes (static singleton problems?)
	destroyFlashControl();
}

void FlashMovie::hide()
{
	if ( m_flashControl )
		m_flashControl->hide();
}

void FlashMovie::show()
{
	if ( m_flashControl )
		m_flashControl->show();
}

void FlashMovie::resetPosition()
{
	if ( m_flashControl )
		m_flashControl->resetPosition();
}

void FlashMovie::rewind()
{
	if ( m_flashControl )
		m_flashControl->rewind();
}

void FlashMovie::focus()
{
	if ( m_flashControl )
		m_flashControl->focus();
}

// Communication
void FlashMovie::callFlashFunction( const Cing::String& name, FlashMovie::Args& arguments )
{
	if ( m_flashControl )
		m_flashControl->callFunction( name, arguments ); 
}

/**
 *	To register a C/C++ function to be called from Flash:
	Function signature: 
		Hikari::FlashValue functionName( Hikari::FlashControl* caller, const Hikari::Arguments& args )

	To Register function
		registerCFunction("functionName", &functionName);

	To Register class method
		registerCFunction("functionName", Hikari::FlashDelegate(this, &ClassName::functionName));
*/
void FlashMovie::registerCFunction	(const Cing::String& funcName, const Hikari::FlashDelegate& callback)
{
	if ( m_flashControl )
		m_flashControl->bind( funcName, callback );
}


Hikari::FlashValue FlashMovie::closeMovie( Hikari::FlashControl* caller, const Hikari::Arguments& args )
{	
	//destroyFlashControl();
	hide();

	// need a return...
	return FLASH_VOID;
}


void FlashMovie::destroyFlashControl()
{
	// Get the hikari manager
	Hikari::HikariManager* hikariMgr = FlashMovieManager::getSingleton().getHikariManager();

	if ( m_flashControl && hikariMgr )
	{
		hikariMgr->destroyFlashControl( m_flashControl );
		m_flashControl = NULL;
		m_loaded = false;
	}
	
}