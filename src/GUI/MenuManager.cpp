/*
	Menu Manager
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "MenuManager.h"

#include "EasyCEGUI/GUIWidget.h"

#include "common/ResourceManager.h"


/**
 * Inits manager resources
 */
void MenuManager::init()
{
	// Make sure we have an active skin (TODO: make this configurable)
	GUIWidget::setSkin("AquaLook");

	m_init = true;
}

/**
 * Releass manager resources
 */
void MenuManager::end()
{
	// Release menus
	for( MenuMap::iterator it = m_menus.begin(); it != m_menus.end(); ++it )
	{
		Release( it->second );
	}
	m_menus.clear();

	m_init = false;
}

/**
 * Update menus
 */
void MenuManager::update()
{
	// Update all menus
	for( MenuMap::iterator it = m_menus.begin(); it != m_menus.end(); ++it )
		it->second->update();
}


/**
 * Loads a menu defined by an xml file.
 */
void MenuManager::loadMenu( const String& xmlFile )
{
	// Check if the MenuManager is init
	if ( !m_init )
		init();

	// if the file exists -> create the menu and load it
	if ( fileExists( ResourceManager::userDataPath + xmlFile ) )
	{
		// Load menu
		Menu *newMenu = new Menu();
		bool loadRes = newMenu->init( xmlFile );

		// Store the menu for future activation
		if ( loadRes )
			m_menus[ newMenu->getName() ]  = newMenu;
	}
	else
		LOG_ERROR( "Menu could not be loaded. File %s not found", xmlFile.c_str() );
}

/**
 * Shows a menu (referenced by its name). The menu will be visible and active.
 */
void MenuManager::showMenu( const String& name )
{
	// Try to find menu
	MenuMap::iterator it = m_menus.find( name );

	// If found -> show it
	if ( it != m_menus.end() )
		it->second->show( true );
	// Menu not found
	else
		LOG_ERROR( "Menu %s does not exist. You should load menu first", name.c_str() );

}
