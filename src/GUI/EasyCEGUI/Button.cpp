#include "Button.h"

/**
 * @internal
 * @brief Constructor 
 */
Button::Button()
{
};

/**
 * @brief Destructor
 *
 */
Button::~Button()
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
void  Button::init( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = BUTTON;

	((GUIWidget*)(this))->init ( (getSkin() + "/" + WIDGET_NAME[m_type]), theName, xPosition, yPosition, xSize, ySize );

	m_window->enable();
};

/**
 * @brief Control transitions here
 */
//void  Button::update()
//{
//	// TODO: Manage transitions
//	// Ref: CeslitMenuModified
//};
