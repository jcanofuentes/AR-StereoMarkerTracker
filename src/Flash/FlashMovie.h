/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#ifndef FLASH_MOVIE_H
#define FLASH_MOVIE_H

#include "common/eString.h"

#include "../Hikari/include/Hikari.h"


// Forward declarations
namespace Hikari 
{
	class HikariManager;
	class FlashControl;
	class Args;
}

class FlashMovie
{
public:
	// Arguments to call Action script functions within the flash movie
	typedef Hikari::Args Args;

	// Construction / Destruction
	FlashMovie(): m_flashControl(NULL), m_loaded(false) { };
	virtual ~FlashMovie() { end(); }

	void load( const Cing::String& flashFile, int width, int height, int xPos, int yPos );
	void end();

	//  Query
	bool isLoaded		() const { return m_loaded; }
	bool isVisible		() const { return m_flashControl? m_flashControl->getVisibility(): false; }
	void hide			();
	void show			();
	void resetPosition	();
	void rewind			();
	void focus			();

	// Hikari
	Hikari::FlashControl* getFlashControl() { return m_flashControl; }

	// Communication
	void				callFlashFunction	( const Cing::String& name, FlashMovie::Args& arguments );
	void				registerCFunction	(const Cing::String& funcName, const Hikari::FlashDelegate& callback);


	// Binded funcitons
	Hikari::FlashValue	closeMovie			( Hikari::FlashControl* caller, const Hikari::Arguments& args );

private:

	// private functions
	void		destroyFlashControl();
	

	// Attributes
	Hikari::FlashControl*	m_flashControl;
	bool					m_loaded;
};

#endif