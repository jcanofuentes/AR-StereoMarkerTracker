#ifndef _CeslitMenu_h
#define _CeslitMenu_h

#define OIS_DYNAMIC_LIB

#include "externLibs/Ogre3d/include/Ogre.h"
#include "externLibs/Ogre3d/include/OIS/OIS.h"
#include "externLibs/Ogre3d/include/cegui/CEGUI.h"
#include "externLibs/Ogre3d/include/cegui/CEGUIBase.h"

static const Ogre::String	DEFAULT_ROOT_NAME				= "CeslitRoot",
													DEFAULT_BACKGROUND_NAME	= "CeslitBackground",
													//DEFAULT_SKIN_NAME				= "Vanilla";
													DEFAULT_SKIN_NAME				= "CeslitLook";
class FaderCallback
{
public:
	virtual void fadeInCallback(void) {}
	virtual void fadeOutCallback(void) {}
};


class CeslitMenu
{
public:
	CeslitMenu() {}

	CeslitMenu(OIS::Mouse *, Ogre::String);
	CeslitMenu(OIS::Mouse* menuMouse, CEGUI::Window* inputCEGUIWindow);
	CeslitMenu(OIS::Mouse* menuMouse, CEGUI::Window* inputCEGUIWindow, FaderCallback *instance);
	~CeslitMenu(void);

	static void setSkin( Ogre::String );
	void createBackground( Ogre::String, CEGUI::UVector2, CEGUI::UVector2  );
	void createButton( Ogre::String, Ogre::String,
										 CEGUI::UVector2, CEGUI::UVector2,
										 CEGUI::SubscriberSlot, Ogre::Real,
										 Ogre::String);

	void createImageButton(	Ogre::String, Ogre::String,
													CEGUI::UVector2, CEGUI::UVector2,
													CEGUI::SubscriberSlot, Ogre::Real,
													Ogre::String);

	void createImage( Ogre::String fileName,CEGUI::UVector2, CEGUI::UVector2 );

	void createSelectionScrollbar( Ogre::String, Ogre::String,
																 CEGUI::UVector2, CEGUI::UVector2,
																 CEGUI::SubscriberSlot, float, float,
															   Ogre::Real, Ogre::String, Ogre::String);

	void update(double timeSinceLastFrame);
	void show();
	void hide();
	void setAlpha( float alpha );
	void processInput();
	//bool quit(const CEGUI::EventArgs&);
	bool HandleEmptyMouseClick(const CEGUI::EventArgs&)
	{
		MessageBoxA(NULL, "Not implemented yet", "CESLIT", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		return true;
	}

	void setBackground( CEGUI::Window* inputWindow ) { menuBackground = inputWindow; };

	CEGUI::Window* rootGUIWindow;
	CEGUI::Window* menuBackground;
	OIS::Mouse*    menuMouse;

	// TODO:
	std::vector< CEGUI::Window* > mImages;
	void showImage(int _id);

	// Fade IN/ OUT Controller
	void startFadeIn(double duration = 1.0f);
	void startFadeOut(double duration = 1.0f);
	void fade(double timeSinceLastFrame);

protected:
	double _alpha;
	double _current_dur;
	double _total_dur;
	FaderCallback* _inst;

	enum _fadeop {
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
	} _fadeop;

private:

	void mouseMoved();
	void mouseLeftClick();
	void mouseLeftReleased();

};

#endif //  _CeslitMenu_h
