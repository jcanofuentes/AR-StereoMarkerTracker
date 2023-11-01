#ifndef _Button_H_
#define _Button_H_

#include "GUIWidget.h"

/**
 * GUI Button
 */
class Button : public GUIWidget
{
public:

	// Constructor / Destructor
	Button	();
	~Button	();

	// Init / Release / Update
	void  init          ( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize );
	//void  update        ();

};

#endif // _Button_H_
