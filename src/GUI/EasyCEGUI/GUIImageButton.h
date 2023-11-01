#ifndef _GUIImageButton_H_
#define _GUIImageButton_H_

#include "GUIWidget.h"

/**
* GUI ImageButton
*/
class GUIImageButton : public GUIWidget
{
public:

	// Constructor / Destructor
	GUIImageButton	();
	~GUIImageButton	();

	// Init / Release / Update
	void  init          ( const Cing::String& theLayoutFile, const Cing::String& theName, float xPosition = 0, float yPosition = 0, float xSize = 800, float ySize = 600 );

private:
	void	init	( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize);
};

#endif // _GUIImageButton_H_
