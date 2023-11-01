#ifndef _GUISlider_H_
#define _GUISlider_H_

#include "GUIWindow.h"

/**
* GUI Static text
*/
class GUISlider : public GUIWindow
{
public:

	// Constructor / Destructor
	GUISlider	();
	~GUISlider	();

	// Init / Release / Update
	void	init		( const Cing::String& theName, float xPosition = 0, float yPosition = 0, float xSize = 800, float ySize = 600 );

	// Set values
	void	setMaxValue		( float max );
	void	setCurrentValue	( float value );
	void	setStep			( float step );

	// Get values
	float	getCurrentValue() const { return m_currentValue; }

	// Events
	bool	sliderChanged( const CEGUI::EventArgs& args );

private:

	float			m_currentValue;
	CEGUI::Slider*	m_slider;
};

#endif // _GUISlider_H_
