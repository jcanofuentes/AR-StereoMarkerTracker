#include "GUIImageButton.h"

//CEGUI
#include "ogre3d/include/cegui/CEGUIExceptions.h"
#include "ogre3d/include/cegui/CEGUIImagesetManager.h"
#include "Ogre3d/include/cegui/CEGUIWindowManager.h"

/**
* @internal
* @brief Constructor 
*/
GUIImageButton::GUIImageButton()
{
};

/**
* @brief Destructor
*
*/
GUIImageButton::~GUIImageButton()
{
	// Release resources
	end();
};

void	GUIImageButton::init( const Cing::String& theLayoutFile, const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = IMAGEBUTTON;

	m_name = theName + "/ImageButton";

	init(theLayoutFile, xPosition, yPosition, xSize, ySize);
}

/**
* @brief Init GUIImage parameters
* @param xPosition	xPosition Button xPos
* @param yPosition	yPosition Button yPos
* @param theImageFile	theImageFile   The name of the GUIImage
*
*/
void  GUIImageButton::init	( const Cing::String& theLayoutFile, float xPosition, float yPosition, float xSize, float ySize)
{
	try
	{
		CEGUI::ImagesetManager::getSingleton().createImageset( theLayoutFile + ".imageset" );
	}
	catch(CEGUI::Exception&) { }
	try
	{
		//ImageButton window must be created with a special call, so GUIWidget generic initialization cannot be used.
		m_window = CEGUI::WindowManager::getSingleton().loadWindowLayout( theLayoutFile + ".layout" );
		m_window->rename(m_name.c_str());

		m_window->setPosition(CEGUI::UVector2(CEGUI::UDim(0, xPosition), CEGUI::UDim(0, yPosition)));
		m_window->enable();
	}
	catch(CEGUI::Exception&) { }

	m_bIsValid = true;
};
