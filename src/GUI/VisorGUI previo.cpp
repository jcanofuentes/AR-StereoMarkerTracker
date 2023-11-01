/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "VisorGUI.h"
#include "Cing.h"

// Crea la GUI para al visor
void VisorGUI::init( Visor& visor ) 
{
	// Almacenar punto al visor
	m_visor = &visor;

	// Skin en uso
	GUIWidget::setSkin("AquaLook");

	// Posiciones y escalas
	int w = 200, h = 290;
	int x = 10, y = 10;


	// Imagen de fondo
	m_background.init("GUI/background.png", 0, 0, width, height);
	m_background.show();
	m_background.useAsBackground();

	// Ventana de controles
	m_window.init( "VisorGUI", x, y, w, h );
	m_window.setAlpha( 0.5f );
	m_window.show();

	// Crear textos y sliders

	// Sagital
	int currentY = 60;
	m_sagital.init( "Sagital", 15, currentY, w-30, 55 );
	m_window.addChild( m_sagital );

	m_sagitalSlider.init( "SagitalSlider", 5, 35, w-40, 10 );
	m_sagital.addChild( m_sagitalSlider );

	// Axial
	currentY = 130;
	m_axial.init( "Axial", 15, currentY, w-30, 55 );
	m_window.addChild( m_axial );

	// Crear Sliders
	m_axialSlider.init( "AxialSlider", 5, 35, w-40, 10 );
	m_axial.addChild( m_axialSlider );


	// Coronal
	currentY = 200;
	m_coronal.init( "Coronal", 15, currentY, w-30, 55 );
	m_window.addChild( m_coronal );

	// Crear Sliders
	m_coronalSlider.init( "CoronalSlider", 5, 35, w-40, 10 );
	m_coronal.addChild( m_coronalSlider );

	// Poner valores iniciales y rango en sliders
	m_sagitalSlider.setCurrentValue( m_visor->getIndexXAxis() );
	m_sagitalSlider.setMaxValue( m_visor->getMaxXAxis() );

	m_axialSlider.setCurrentValue( m_visor->getIndexYAxis() );
	m_axialSlider.setMaxValue( m_visor->getMaxYAxis() );

	m_coronalSlider.setCurrentValue( m_visor->getIndexZAxis() );
	m_coronalSlider.setMaxValue( m_visor->getMaxZAxis() );

	// Poner textos con estos valores	
	m_sagital.setText( "Sagital: " + intToString( m_visor->getIndexXAxis() ) );
	m_axial.setText( "Axial: " + intToString( m_visor->getIndexYAxis() ) );
	m_coronal.setText( "Coronal: " + intToString( m_visor->getIndexZAxis() ) );

}

// Libera recursos de la GUI
void VisorGUI::end() 
{
}

void VisorGUI::update() 
{
	m_window.update();

	// Upadte los valores del visor
	m_visor->setIndexXAxis( m_sagitalSlider.getCurrentValue() );
	m_visor->setIndexYAxis( m_axialSlider.getCurrentValue() );
	m_visor->setIndexZAxis( m_coronalSlider.getCurrentValue() );

	// Update de textos
	m_sagital.setText( "Sagital: " + intToString( m_visor->getIndexXAxis() ) );
	m_axial.setText( "Axial: " + intToString( m_visor->getIndexYAxis() ) );
	m_coronal.setText( "Coronal: " + intToString( m_visor->getIndexZAxis() ) );
}