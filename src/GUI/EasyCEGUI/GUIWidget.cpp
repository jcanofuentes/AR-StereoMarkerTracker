#include "GUIWidget.h"

//Cing::GUI
#include "common/CommonUserAPI.h"
#include "framework/UserAppGlobals.h"
#include "GUI/GUIManagerCEGUI.h"

//Ogre
#include "Ogre3d/include/OgreException.h"

//CEGUI
#include "Ogre3d/include/cegui/CEGUIExceptions.h"
#include "Ogre3d/include/cegui/CEGUISchemeManager.h"
#include "Ogre3d/include/cegui/CEGUIWindowManager.h"
#include "Ogre3d/include/CEGUI/elements/CEGUIPushButton.h"


using namespace Cing;


String GUIWidget::m_skinName = DEFAULT_SKIN;

/**
 * @internal
 * @brief Constructor 
 */
GUIWidget::GUIWidget() : m_bIsValid (false), m_window(NULL)
{};

/**
 * @brief Destructor
 *
 */
GUIWidget::~GUIWidget()
{
	// Release resources
	end();
};

/**
 * @brief Init window parameters
 * @param xPosition	xPosition GUIWidget xPos
 * @param yPosition	yPosition GUIWidget yPos
 * @param theName	theName   The name of the GUIWidget
 *
 */
void  GUIWidget::init(const Cing::String& theType, const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize )
{
	m_name = theName;

	m_window = CEGUI::WindowManager::getSingleton().createWindow(theType, m_name);

	m_window->setPosition(CEGUI::UVector2(CEGUI::UDim(0, xPosition), CEGUI::UDim(0, yPosition)));
	m_window->setSize(CEGUI::UVector2(CEGUI::UDim(0, xSize), CEGUI::UDim(0, ySize)));
	//m_window->disable();//By default a widget is disabled, so the subtypes that must be able to receive input should enable themselves on initialization.

	m_bIsValid = true;
};

/**
 * @brief Init window parameters
 * @param xPosition	xPosition GUIWidget xPos
 * @param yPosition	yPosition GUIWidget yPos
 * @param theName	theName   The name of the GUIWidget
 *
 */
void  GUIWidget::init(CEGUI::Window* window, float xPosition, float yPosition, float xSize, float ySize )

{
	m_name = window->getName().c_str();

	m_window = window;

	m_window->setPosition(CEGUI::UVector2(CEGUI::UDim(0, xPosition), CEGUI::UDim(0, yPosition)));
	m_window->setSize(CEGUI::UVector2(CEGUI::UDim(0, xSize), CEGUI::UDim(0, ySize)));
	//m_window->disable();//By default a widget is disabled, so the subtypes that must be able to receive input should enable themselves on initialization.

	m_bIsValid = true;
};
/**
 * @internal
 * @brief Releases the class resources.
 * After this call no method of this object can be called without calling init method again.
 */
void  GUIWidget::end()
{
	// TODO

	m_bIsValid = false;
};

/**
 * @brief Control transitions here
 */
void  GUIWidget::update()
{
	if (_fadeop != FADE_NONE) fade(Cing::elapsedSec);
};

Cing::String GUIWidget::getSkin()
{
	return m_skinName;
};

void GUIWidget::setSkin(Cing::String skinName)
{
	m_skinName = skinName;

	try
	{
		CEGUI::SchemeManager::getSingleton().loadScheme(m_skinName + "Skin.scheme");
	}
	catch(Ogre::FileNotFoundException e)
	{
		//Cing log "File not found"
		//Cing::L
	}
	catch(CEGUI::AlreadyExistsException e)
	{
		//Cing log "Skin " + schemeName + " already loaded", "Informative"
		//CEGUI::Logger::getSingletonPtr()->logEvent("Skin " + schemeName + " already loaded", CEGUI::Informative);
	}
};

Cing::String GUIWidget::getName() const
{
	return m_name;
};

/**
* @brief Make the widget visible
*/
void GUIWidget::show()
{
	m_window->enable();
	m_window->show();
	Cing::GUIManagerCEGUI::getSingleton().addGUIELement( m_window );
};

const CEGUI::Window* GUIWidget::getGEGUIWindow() const
{
	return m_window;
};

void GUIWidget::setPushCallback(GUIFunctor::GUICallback callback, void *operand)
{
	m_window->removeEvent(CEGUI::PushButton::EventClicked);

	if (!m_listener) m_listener = new GUIListener();

	m_listener->setPushCallback(callback, operand);

	m_window->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::SubscriberSlot(&GUIListener::executePushed, m_listener));
};

void GUIWidget::setFadeInCallback(GUIFunctor::GUICallback callback, void *operand)
{
	if (!m_listener) m_listener = new GUIListener();

	m_listener->setFadeInCallback(callback, operand);
}

void GUIWidget::setFadeOutCallback(GUIFunctor::GUICallback callback, void *operand)
{
	if (!m_listener) m_listener = new GUIListener();

	m_listener->setFadeOutCallback(callback, operand);
}

void GUIWidget::setListener(GUIListener &listener)
{
	if (m_listener) delete (m_listener);

	m_listener = &listener;
}

void GUIWidget::setAlpha( float alpha)
{
	if ( m_window )
		m_window->setAlpha(alpha);
}

void GUIWidget::startFadeIn(double duration )
{
	if( duration < 0 )
		duration = -duration;
	if( duration < 0.000001 )
		duration = 1.0;

	_alpha = 0.0;
	_total_dur = duration;
	_fadeop = FADE_IN;
	m_window->setAlpha(0.0);
	m_window->show();
	this->show();
}

void GUIWidget::startFadeOut(double duration )
{
	if( duration < 0 )
		duration = -duration;
	if( duration < 0.000001 )
		duration = 1.0;

	_alpha = 1.0;
	_total_dur = duration;
	_fadeop = FADE_OUT;
	m_window->setAlpha(1.0);
	m_window->show();
}

void GUIWidget::fade(double timeSinceLastFrame)
{
	if( _fadeop != FADE_NONE  )
	{
		// Set the _alpha value of the widget
		// If fading out, decrease the _alpha until it reaches 0.0
		if( _fadeop == FADE_OUT )
		{
			_alpha -= (timeSinceLastFrame / _total_dur);
			if( _alpha <= 0.0 )
			{
				_alpha = 0.0;
				m_window->setAlpha(_alpha);
				m_window->hide();
				m_window->disable();
				_fadeop = FADE_NONE;
				if (m_listener) m_listener->executeFadeOutEnded();
			}
			else m_window->setAlpha( _alpha );
		}

		// If fading in, increase the _alpha until it reaches 1.0
		else if( _fadeop == FADE_IN )
		{
			_alpha += (timeSinceLastFrame / _total_dur);
			if( _alpha >= 1.0 )
			{
				_alpha = 1.0;
				m_window->setAlpha(_alpha);
				_fadeop = FADE_NONE;
				if (m_listener) m_listener->executeFadeInEnded();
			}
			else m_window->setAlpha( _alpha );
		}
	}
}
