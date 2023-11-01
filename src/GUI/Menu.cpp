/*
	Menu 
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "Menu.h"

#include "EasyCEGUI/GUIWindow.h"
#include "EasyCEGUI/GUIStaticText.h"

#include "Ogre3d/include/cegui/CEGUI.h"



// Static members
long long Menu::m_widgetCount = 0;

/**
 * Inits manager resources
 */
bool Menu::init( const String& xmlFile )
{
	// Init general attributes
	m_activeWindow = 0;

	// Load xml file
	XMLElement xml;
	xml.load( xmlFile );

	// Load genera menu properties
	m_name			= xml.getStringAttribute( "name" );
	m_position.x	= xml.getFloatAttribute( "posX" );
	m_position.y	= xml.getFloatAttribute( "posY" );
	m_dimension.x	= xml.getFloatAttribute( "dimX" );
	m_dimension.y	= xml.getFloatAttribute( "dimY" );
	float opacity	= xml.getFloatAttribute( "opacity" );

	// Check menu has a name
	if ( m_name == "" )
	{
		LOG_ERROR( "Error loading menu: %s. It has no name field" );
		return false;
	}

	// Load layout
	XMLElement layout = xml.getChild( "layout" );
	String layoutFile = layout.getStringAttribute( "file" );

	//Load up our player windows	
	CEGUI::WindowManager&	winMgr	= CEGUI::WindowManager::getSingleton();
	CEGUI::Window *rootWindow		= winMgr.loadWindowLayout( dataFolder +  layoutFile );

	// Add window to gui
	Cing::GUIManagerCEGUI::getSingleton().addGUIELement( rootWindow );

	// Load text


	// Load menu windows
	XMLElementArray windows, widgets;
	layout.getChildren( windows, "window" );
	for ( size_t i = 0; i < windows.size(); ++i )
	{
		// Get window name
		String name = windows[i].getStringAttribute( "name" );

		// Get window pointer
		CEGUI::Window* newWindowCEGUI = rootWindow->getChild( name );
		newWindowCEGUI->disable();
		newWindowCEGUI->hide();

		// Create the window
		GUIWindow* newWindow = new GUIWindow();
		newWindow->init( newWindowCEGUI, m_position.x, m_position.y, m_dimension.x, m_dimension.y );
		newWindow->setAlpha( opacity );

		//// Load widgets of this window
		//widgets.clear();
		//windows[i].getChildren( widgets, "widget" );
		//for ( size_t j = 0; j < widgets.size(); ++j )
		//{
		//	// Widget type
		//	String type = widgets[j].getStringAttribute( "type" );

		//	// Build depending on type
		//	if ( type == "TextField" )
		//	{
		//		// Get attributes
		//		float posX	= widgets[j].getFloatAttribute( "posX" );
		//		float posY	= widgets[j].getFloatAttribute( "posY" );
		//		float dimX	= widgets[j].getFloatAttribute( "dimX" );
		//		float dimY	= widgets[j].getFloatAttribute( "dimY" );
		//		String text = widgets[j].getStringAttribute( "text" );

		//		GUIStaticText newText;
		//		newText.init( "TextField" + intToString( m_widgetCount++ ), posX, posY, dimX, dimY );
		//		newText.setText( text );
		//		newWindow->addChild( newText );
			//}
		//}

		// Store the window
		m_windows.push_back( newWindow );

	}

	// all ok
	return true;
}
//
//bool Menu::init( const String& xmlFile )
//{
//	// Init general attributes
//	m_activeWindow = 0;
//
//	// Load xml file
//	XMLElement xml;
//	xml.load( xmlFile );
//
//	// Load genera menu properties
//	m_name			= xml.getStringAttribute( "name" );
//	m_position.x	= xml.getFloatAttribute( "posX" );
//	m_position.y	= xml.getFloatAttribute( "posY" );
//	m_dimension.x	= xml.getFloatAttribute( "dimX" );
//	m_dimension.y	= xml.getFloatAttribute( "dimY" );
//	float opacity	= xml.getFloatAttribute( "opacity" );
//
//	// Check menu has a name
//	if ( m_name == "" )
//	{
//		LOG_ERROR( "Error loading menu: %s. It has no name field" );
//		return false;
//	}
//
//	// Load layout
//	XMLElement layout = xml.getChild( "layout" );
//	String layoutFile = layout.getStringAttribute( "file" );
//
//	//Load up our player windows	
//	CEGUI::WindowManager&	winMgr	= CEGUI::WindowManager::getSingleton();
//	CEGUI::Window *rootWindow		= winMgr.loadWindowLayout( dataFolder +  layoutFile );
//
//	// Add window to gui
//	Cing::GUIManagerCEGUI::getSingleton().addGUIELement( rootWindow );
//
//	// Load menu windows
//	XMLElementArray windows, widgets;
//	layout.getChildren( windows, "window" );
//	for ( size_t i = 0; i < windows.size(); ++i )
//	{
//		// Get window name
//		String name = windows[i].getStringAttribute( "name" );
//
//		// Get window pointer
//		CEGUI::Window* newWindowCEGUI = rootWindow->getChild( name );
//		newWindowCEGUI->disable();
//		newWindowCEGUI->hide();
//
//		// Create the window
//		GUIWindow* newWindow = new GUIWindow();
//		newWindow->init( newWindowCEGUI, m_position.x, m_position.y, m_dimension.x, m_dimension.y );
//		newWindow->setAlpha( opacity );
//
//		//// Load widgets of this window
//		//widgets.clear();
//		//windows[i].getChildren( widgets, "widget" );
//		//for ( size_t j = 0; j < widgets.size(); ++j )
//		//{
//		//	// Widget type
//		//	String type = widgets[j].getStringAttribute( "type" );
//
//		//	// Build depending on type
//		//	if ( type == "TextField" )
//		//	{
//		//		// Get attributes
//		//		float posX	= widgets[j].getFloatAttribute( "posX" );
//		//		float posY	= widgets[j].getFloatAttribute( "posY" );
//		//		float dimX	= widgets[j].getFloatAttribute( "dimX" );
//		//		float dimY	= widgets[j].getFloatAttribute( "dimY" );
//		//		String text = widgets[j].getStringAttribute( "text" );
//
//		//		GUIStaticText newText;
//		//		newText.init( "TextField" + intToString( m_widgetCount++ ), posX, posY, dimX, dimY );
//		//		newText.setText( text );
//		//		newWindow->addChild( newText );
//			//}
//		//}
//
//		// Store the window
//		m_windows.push_back( newWindow );
//
//	}
//
//	// all ok
//	return true;
//}
//bool Menu::init( const String& xmlFile )
//{
//	// Init general attributes
//	m_activeWindow = 0;
//
//	// Load xml file
//	XMLElement xml;
//	xml.load( xmlFile );
//
//	// Load genera menu properties
//	m_name			= xml.getStringAttribute( "name" );
//	m_position.x	= xml.getFloatAttribute( "posX" );
//	m_position.y	= xml.getFloatAttribute( "posY" );
//	m_dimension.x	= xml.getFloatAttribute( "dimX" );
//	m_dimension.y	= xml.getFloatAttribute( "dimY" );
//	float opacity	= xml.getFloatAttribute( "opacity" );
//
//	// Check menu has a name
//	if ( m_name == "" )
//	{
//		LOG_ERROR( "Error loading menu: %s. It has no name field" );
//		return false;
//	}
//
//	// Load menu windows
//	XMLElementArray windows, widgets;
//	xml.getChildren( windows, "window" );
//	for ( size_t i = 0; i < windows.size(); ++i )
//	{
//		// Get window title
//		String title = windows[i].getStringAttribute( "title" );
//
//		// Create the window
//		GUIWindow* newWindow = new GUIWindow();
//		newWindow->init( title, m_position.x, m_position.y, m_dimension.x, m_dimension.y );
//		newWindow->setAlpha( opacity );
//
//		// Load widgets of this window
//		widgets.clear();
//		windows[i].getChildren( widgets, "widget" );
//		for ( size_t j = 0; j < widgets.size(); ++j )
//		{
//			// Widget type
//			String type = widgets[j].getStringAttribute( "type" );
//
//			// Build depending on type
//			if ( type == "TextField" )
//			{
//				// Get attributes
//				float posX	= widgets[j].getFloatAttribute( "posX" );
//				float posY	= widgets[j].getFloatAttribute( "posY" );
//				float dimX	= widgets[j].getFloatAttribute( "dimX" );
//				float dimY	= widgets[j].getFloatAttribute( "dimY" );
//				String text = widgets[j].getStringAttribute( "text" );
//
//				GUIStaticText newText;
//				newText.init( "TextField" + intToString( m_widgetCount++ ), posX, posY, dimX, dimY );
//				newText.setText( text );
//				newWindow->addChild( newText );
//			}
//		}
//
//		// Store the window
//		m_windows.push_back( newWindow );
//
//	}
//
//	// all ok
//	return true;
//}

/**
 * Releass manager resources
 */
void Menu::end()
{
	// Release windows
	for ( size_t i = 0; i < m_windows.size(); ++i )
		Release( m_windows[i] );
	m_windows.clear();
}

/**
 * Update menus
 */
void Menu::update()
{
	// Update all windows
	if ( m_active )
	{
		std::for_each( m_windows.begin(), m_windows.end(), std::mem_fun( &GUIWindow::update ) );
	}
}

/**
 * Shows or hides the menu (fades in and out by defautl)
 */
void Menu::show( bool show )
{
	m_active = show;

	if ( show )
		m_windows[m_activeWindow]->show();
	// TODO review fade problems (with listeners...)
		//m_windows[m_activeWindow]->startFadeIn( 0.5 );
	else 
		m_windows[m_activeWindow]->startFadeOut( 0.5 );
}
