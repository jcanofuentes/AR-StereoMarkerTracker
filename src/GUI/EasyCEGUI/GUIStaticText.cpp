#include "GUIStaticText.h"


/**
* @internal
* @brief Constructor 
*/
GUIStaticText::GUIStaticText()
{
};

/**
* @brief Destructor
*
*/
GUIStaticText::~GUIStaticText()
{
	// Release resources
	end();
};

void GUIStaticText::init( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = STATIC_TEXT;
	m_name = theName + "/StaticText";

	GUIWidget::init ( (getSkin() + "/" + WIDGET_NAME[m_type]), theName, xPosition, yPosition, xSize, ySize );
	m_window->enable();

	// Set text and color (black by default)
	m_window->setText( theName );
	m_window->setProperty("TextColours", "tl:FF000000  tr:FF000000  bl:FF000000  br:FF000000");
	m_window->setProperty("VertFormatting", "TopAligned");
	m_window->setProperty("HorzFormatting", "LeftAligned");
}


void GUIStaticText::setText( const Cing::String& text )
{
	if ( m_window )
		m_window->setText( text );

}
