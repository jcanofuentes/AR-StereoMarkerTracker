#include "GUIImage.h"

//CEGUI
#include "ogre3d/include/cegui/CEGUIImagesetManager.h"

/**
* @internal
* @brief Constructor 
*/
GUIImage::GUIImage()
{
};

/**
* @brief Destructor
*
*/
GUIImage::~GUIImage()
{
	// Release resources
	end();
};

/**
* @brief Init GUIImage parameters
* @param xPosition	xPosition Button xPos
* @param yPosition	yPosition Button yPos
* @param theImageFile	theImageFile   The name of the GUIImage
*
*/
void  GUIImage::init( const Cing::String& theImageFile, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = IMAGE;

	((GUIWidget*)(this))->init ( (getSkin() + "/" + WIDGET_NAME[m_type]), theImageFile, xPosition, yPosition, xSize, ySize );

	CEGUI::ImagesetManager::getSingleton().createImagesetFromImageFile("ImageForStaticImage" + theImageFile, theImageFile );
	m_window->setProperty("Image", "set:ImageForStaticImage" + theImageFile + " image:full_image"); // "full_image" is a default name from CEGUIImageset::Imageset()
	m_window->setProperty( "BackgroundEnabled", "False" );
	m_window->disable();
};

/**
* @brief Control transitions here
*/
//void  GUIImage::update()
//{
//	// TODO: Manage transitions
//	// Ref: CeslitMenuModified
//};

void GUIImage::useAsBackground()
{
	//m_window->setZOrderingEnabled( false );
	m_window->setProperty("AlwaysOnTop", "False");
	m_window->moveToBack();
	m_window->setRiseOnClickEnabled( false );
}
