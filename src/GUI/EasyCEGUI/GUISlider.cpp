#include "GUISlider.h"

//CEGUI
#include "ogre3d/include/cegui/elements/CEGUISlider.h"

/**
* @internal
* @brief Constructor 
*/
GUISlider::GUISlider()
{
};

/**
* @brief Destructor
*
*/
GUISlider::~GUISlider()
{
	// Release resources
	end();
};

void GUISlider::init( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_type = SLIDER;
	m_name = theName + "/Slider";

	GUIWidget::init ( (getSkin() + "/" + WIDGET_NAME[m_type]), theName, xPosition, yPosition, xSize, ySize );
	m_window->enable();

	// Store pointer to slider
	m_slider = (CEGUI::Slider*)m_window;

	// Suscribe to slider change event
	m_slider->subscribeEvent( CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber( &GUISlider::sliderChanged, this ) );

	// Set initial values
	m_slider->setMaxValue( 100 );
	m_slider->setCurrentValue( 1 );
	m_slider->setClickStep( 1 );
}


void GUISlider::setMaxValue( float max )
{
	if ( !m_slider )
		return;

	m_slider->setMaxValue( max );
}

void GUISlider::setCurrentValue( float value )
{
	if ( !m_slider )
		return;

	m_slider->setCurrentValue( value );
}

void GUISlider::setStep ( float step )
{
	if ( !m_slider )
		return;

	m_slider->setClickStep( step );
}

bool GUISlider::sliderChanged( const CEGUI::EventArgs& args )
{
	if ( !m_slider )
		return true;

	m_currentValue = ((CEGUI::Slider*)m_window)->getCurrentValue();
	return true;
}
