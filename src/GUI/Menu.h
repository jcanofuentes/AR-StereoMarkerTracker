/*
	Menu
	Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
*/

#include "cing.h"

// Forward declarations
class GUIWindow;


class Menu
{
public:
	Menu(): m_active(false) { };
	~Menu() { end(); }

	bool init	( const String& xmlFile );
	void end	();
	void update	();

	// Get/Set methods
	const String&	getName	()				{ return m_name; }
	void			show	( bool show );

private:

	// To create unique widget ids
	static long long			m_widgetCount;

	// Attributes
	String						m_name;
	Vector						m_position;
	Vector						m_dimension;
	std::vector< GUIWindow*>	m_windows;
	size_t						m_activeWindow;
	bool						m_active;
};