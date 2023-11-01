#ifndef _GUIWidget_H_
#define _GUIWidget_H_

#include "GUIFunctor.h"
#include "GUIListener.h"

// Common 
#include "Common/eString.h"

// CEGUI 
#include "Ogre3d/include/cegui/CEGUIInputEvent.h"
#include "Ogre3d/include/cegui/CEGUIWindow.h"

static const Cing::String DEFAULT_SKIN = "Vanilla";

enum WidgetType
{
	WINDOW,
	BUTTON,
	IMAGE,
	IMAGEBUTTON,
	STATIC_TEXT,
	SLIDER
};

// Enum for fadein/out control
enum FadeOp
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT
};

static const Cing::String WIDGET_NAME[] =
{
	"Window",
	"Button",
	"StaticImage",
	"ImageButton",
	"StaticText",
	"SliderHoriz"
};


/**
 * GUIWidget
 */
class GUIWidget
{
	friend class Button;
	friend class GUIWindow;
	friend class GUIImage;
	friend class GUIImageButton;

public:

	static Cing::String getSkin();
	static void setSkin(Cing::String skinName);

	virtual void	init	( const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize) = 0;
	void			update        ();
	void			end           ();
	Cing::String	getName() const;
	// Show a window
	void			show	();
	const CEGUI::Window*	getGEGUIWindow() const;

	void setPushCallback( GUIFunctor::GUICallback callback, void *operand = NULL );
	void setFadeInCallback( GUIFunctor::GUICallback callback, void *operand = NULL);
	void setFadeOutCallback( GUIFunctor::GUICallback callback, void *operand = NULL);

	void startFadeIn(double duration = 1.0f);
	void startFadeOut(double duration = 1.0f);

	void setListener(GUIListener &listener);

	void setAlpha( float alpha);

protected:

	// Constructor / Destructor
	GUIWidget	();
	virtual ~GUIWidget	();

	// Init / Release / Update
	void  init          (const Cing::String& theType, const Cing::String& theName, float xPosition, float yPosition, float xSize, float ySize );
	void  init          (CEGUI::Window* window, float xPosition, float yPosition, float xSize, float ySize );

	// Fade IN/ OUT Controller
	void fade(double timeSinceLastFrame);

	GUIListener*		m_listener;

	CEGUI::Window*		m_window;
	Cing::String        m_name;
	bool				m_bIsValid;
	WidgetType			m_type;

	static Cing::String	m_skinName;


	double _alpha;
	double _total_dur;

	enum _fadeop {
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
	} _fadeop;
};

#endif // _GUIWidget_H_
