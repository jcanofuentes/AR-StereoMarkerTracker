#ifndef _GUIWindow_H_
#define _GUIWindow_H_

#include "GUIWidget.h"


/**
 * GUI Window
 */
class GUIWindow : public GUIWidget
{
public:

	// Constructor / Destructor
	GUIWindow	();
	~GUIWindow	();

	// Add a new child GUI object
	void  addChild      ( const GUIWidget& theWidget );

	// Remove a GUI object
	//void  removeChild   ( int childId );

	// Add a background image to the window
	void	addBackground(Cing::String fileName);
	void	addBackground(GUIImage &background);

	// Init / Release / Update
	void  init          ( const Cing::String& theName, float xPosition = 0, float yPosition = 0, float xSize = 800, float ySize = 600);
	void  init			( CEGUI::Window* window, float xPosition, float yPosition, float xSize, float ySize );

	void  update        ();

	// Other
	// TODO: Implementar
	//void  addListener   ( GUIListener listener );

private:
	std::vector<GUIWidget*> children;
};

#endif // _GUIWindow_H_
