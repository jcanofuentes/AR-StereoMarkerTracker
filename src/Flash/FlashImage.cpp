/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "graphics/GraphicsManager.h"
#include "framework/UserAppGlobals.h"


#include "FlashImage.h"



// Loads a flash movie player (swf)
void FlashImage::load( const Cing::String& imageFileName, int width, int height, int xPos, int yPos )
{
	// Load swf file
	FlashMovie::load( "fImage.swf", width, height, xPos, yPos );
	
	// Build full path to flash movie (all slashes -> \\ and no ..)
	Cing::String path = Cing::dataFolder + imageFileName;
	path.replaceSubStr( "\\", "\\\\" );
	path.replaceSubStr( "/", "\\\\" );
	path.replaceSubStr( "..", "" );
	path.replaceSubStr( "Debug", "" );
	path.replaceSubStr( "Release", "" );
	path.replaceSubStr( "\\\\\\\\", "\\\\" );
	path.replaceSubStr( "\\\\\\\\", "\\\\" );

	// Call the flash function to load the movie file
	callFlashFunction("loadGalleryXML", FlashMovie::Args( path.c_str() )); 
}