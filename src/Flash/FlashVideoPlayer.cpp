/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "graphics/GraphicsManager.h"
#include "framework/UserAppGlobals.h"


#include "FlashVideoPlayer.h"



// Loads a flash movie player (swf)
void FlashVideoPlayer::load( const Cing::String& flvMovieFile, int width, int height, int xPos, int yPos )
{
	// If its already loaded do nothing...
	if ( isLoaded() )
		return;

	// Load swf file
	FlashMovie::load( "fPlayerIn.swf", width, height, xPos, yPos );
	

	// Build full path to flash movie (all slashes -> \\ and no ..)
	Cing::String path = Cing::dataFolder + flvMovieFile;
	path.replaceSubStr( "\\", "\\\\" );
	path.replaceSubStr( "/", "\\\\" );
	path.replaceSubStr( "..", "" );
	path.replaceSubStr( "Debug", "" );
	path.replaceSubStr( "Release", "" );
	//path.replaceSubStr( "AbadiaNet", "" );
	path.replaceSubStr( "\\\\\\\\", "\\\\" );
	path.replaceSubStr( "\\\\\\\\", "\\\\" );

	// Call the flash function to load the movie file
	callFlashFunction("loadFLV", FlashMovie::Args( path.c_str() )); 
}