/*
	Visor Resonancias
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#ifndef VISOR_GUI_H
#define VISOR_GUI_H


#include "EasyCEGUI/GUIWindow.h"
#include "EasyCEGUI/GUIImage.h"
#include "EasyCEGUI/GUIStaticText.h"
#include "EasyCEGUI/GUISlider.h"

#include "../Visor.h"

class VisorGUI
{
public:
	VisorGUI() { };
	~VisorGUI() { end(); }

	void init( Visor& visor );
	void end();
	void update();

private:
	GUIWindow		m_window;
	GUIImage		m_background;
	GUIStaticText	m_sagital, m_coronal, m_axial;
	GUISlider		m_sagitalSlider, m_coronalSlider, m_axialSlider;

	Visor*			m_visor;
};

#endif