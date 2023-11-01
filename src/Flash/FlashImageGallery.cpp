/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "graphics/GraphicsManager.h"
#include "framework/UserAppGlobals.h"


#include "FlashImageGallery.h"



// Loads a flash movie player (swf)
// NOTE: now it loads the files found in featured_flash.xml
void FlashImageGallery::load( int width, int height, int xPos, int yPos )
{
	// If its already loaded do nothing...
	if ( isLoaded() )
		return;

	// Load swf file
	FlashMovie::load( "AbadiaNet_AR_Gallery.swf", width, height, xPos, yPos );	
}

void FlashImageGallery::loadGalleryXml( const Cing::String& xmlFileName )
{
	// Call the flash function to load the movie file
	callFlashFunction("loadGalleryXML", FlashMovie::Args( xmlFileName.c_str() )); 
}