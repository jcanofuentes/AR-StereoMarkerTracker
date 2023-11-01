#include "GUIWindow.h"

#include "GUIImage.h"

// GUI
#include "GUI/GUIManagerCEGUI.h"

// CEGUI
#include "ogre3d/include/cegui/CEGUIWindowManager.h"


/**
 * @internal
 * @brief Constructor 
 */
GUIWindow::GUIWindow()
{
};

/**
 * @brief Destructor
 *
 */
GUIWindow::~GUIWindow()
{
	// Release resources
	end();
};

/**
 * @brief Init Button parameters
 * @param xPosition	xPosition Button xPos
 * @param yPosition	yPosition Button yPos
 * @param theName	theName   The name of the Button
 *
 */
void  GUIWindow::init( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = WINDOW;

	((GUIWidget*)(this))->init ( "AquaLook/FrameWindow", theName, xPosition, yPosition, xSize, ySize );
};

/**
 * @brief Init Window (from an already created CEGUI window)
 * @param xPosition	xPosition Button xPos
 * @param yPosition	yPosition Button yPos
 * @param theName	theName   The name of the Button
 *
 */
void  GUIWindow::init( CEGUI::Window* window, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = WINDOW;

	GUIWidget::init( window, xPosition, yPosition, xSize, ySize );
};


/**
 * @brief Control transitions here
 */
void  GUIWindow::update()
{
	((GUIWidget*)this)->update();

	for (std::vector<GUIWidget*>::iterator i = children.begin(); i != children.end(); i++)
		(*i)->update();
};

/**
 * @brief Add a new child GUI object
 * @param theWidget	theWidget The reference to the child GUIWidget
 */
void  GUIWindow::addChild( const GUIWidget& theWidget )
{
	children.push_back((GUIWidget *const)&theWidget);
	this->m_window->addChildWindow((CEGUI::String)theWidget.getName());
};

/**
 * @brief Remove a child GUI object
 */
//void  GUIWindow::removeChild( int childId )
//{
//	// TODO
//};

/**
* @brief Add a background image to the window
*/
void GUIWindow::addBackground(Cing::String fileName)
{
	GUIImage background;

	background.init(fileName);
	addBackground(background);
};

void GUIWindow::addBackground(GUIImage &background)
{
	background.useAsBackground();
	addChild(background);
}
