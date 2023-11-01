/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#ifndef FLASH_IMGE_H
#define FLASH_IMGE_H

#include "common/eString.h"
#include "FlashMovie.h"


class FlashImage: public FlashMovie
{
public:

	void load( const Cing::String& imageFileName, int width, int height, int xPos, int yPos );

private:
};

#endif