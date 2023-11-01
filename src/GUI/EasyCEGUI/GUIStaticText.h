#ifndef _GUIStaticText_H_
#define _GUIStaticText_H_

#include "GUIWindow.h"

/**
* GUI Static text
*/
class GUIStaticText : public GUIWindow
{
public:

	// Constructor / Destructor
	GUIStaticText	();
	~GUIStaticText	();

	// Init / Release / Update
	void	init		( const Cing::String& theName, float xPosition = 0, float yPosition = 0, float xSize = 800, float ySize = 600 );

	// Set methods
	void	setText		( const Cing::String& text );

private:
};

#endif // _GUIStaticText_H_
