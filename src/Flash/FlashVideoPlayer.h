/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#ifndef FLASH_VIDEO_PLAYER_H
#define FLASH_VIDEO_PLAYER_H

#include "common/eString.h"
#include "FlashMovie.h"


class FlashVideoPlayer: public FlashMovie
{
public:

	void load( const Cing::String& flvMovieFile, int width, int height, int xPos, int yPos );

private:
};

#endif