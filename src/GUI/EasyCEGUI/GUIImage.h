#ifndef _GUIImage_H_
#define _GUIImage_H_

#include "GUIWidget.h"

/**
* GUI Image
*/
class GUIImage : public GUIWidget
{
public:

	// Constructor / Destructor
	GUIImage	();
	~GUIImage	();

	// Init / Release / Update
	void  init          ( const Cing::String& theImageFile, float xPosition = 0, float yPosition = 0, float xSize = 800, float ySize = 600 );
	//void  update        ();

	void useAsBackground();
};

#endif // _Button_H_
