/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#ifndef FLASH_IMAGE_GALLERY_H
#define FLASH_IMAGE_GALLERY_H

#include "common/eString.h"
#include "FlashMovie.h"


class FlashImageGallery: public FlashMovie
{
public:

	void load			( int width, int height, int xPos, int yPos );
	void loadGalleryXml	( const Cing::String& xmlFileName );

private:
};

#endif