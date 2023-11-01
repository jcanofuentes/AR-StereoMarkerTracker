/*
	Menu Manager
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "Menu.h"

class MenuManager
{
public:
	MenuManager(): m_init(false) { };
	~MenuManager() { end(); }

	void init	();
	void end	();
	void update	();

	// Menu control
	void loadMenu( const String& xmlFile );
	void showMenu( const String& name );

private:
	typedef std::map< String, Menu* > MenuMap;

	// Attributes
	MenuMap		m_menus;
	bool		m_init;
};