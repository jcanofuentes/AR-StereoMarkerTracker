#include "CeslitMenu.h"

using namespace CEGUI;

static float				cursorSpeed = 0.003;
static Ogre::String	schemeName;

CeslitMenu::CeslitMenu(OIS::Mouse* menuMouse, Ogre::String rootGUIWindowName = DEFAULT_ROOT_NAME)
{
	menuBackground = NULL;

	//Create a root window
	rootGUIWindow = WindowManager::getSingleton().createWindow("DefaultWindow", rootGUIWindowName);

	System::getSingletonPtr()->setGUISheet(rootGUIWindow);//The new root window is set as the main top window in the hierarchy

	this->menuMouse = menuMouse;

	return;
}

CeslitMenu::CeslitMenu(OIS::Mouse* menuMouse, CEGUI::Window* inputCEGUIWindow)
{
	menuBackground = NULL;

	//Create a root window
	rootGUIWindow = inputCEGUIWindow;

	System::getSingletonPtr()->setGUISheet(rootGUIWindow);//The new root window is set as the main top window in the hierarchy

	this->menuMouse = menuMouse;

	_fadeop = FADE_NONE;
	_alpha = 0.0;

	return;
}

CeslitMenu::CeslitMenu(OIS::Mouse* menuMouse, CEGUI::Window* inputCEGUIWindow, FaderCallback* instance)
{
	menuBackground = NULL;

	//Create a root window
	rootGUIWindow = inputCEGUIWindow;

	System::getSingletonPtr()->setGUISheet(rootGUIWindow);//The new root window is set as the main top window in the hierarchy

	this->menuMouse = menuMouse;

	_fadeop = FADE_NONE;
	_alpha = 0.0;
	_inst  = instance;
	return;
}

CeslitMenu::~CeslitMenu(void)
{
	//delete rootGUIWindow;//Deletes all the rest of the windows?
}

void CeslitMenu::setSkin(Ogre::String inputSchemeName = DEFAULT_SKIN_NAME)
{
	schemeName = inputSchemeName;

	try
	{
		// load in the scheme file, which auto-loads the TaharezLook imageset
		CEGUI::SchemeManager::getSingleton().loadScheme(schemeName + "Skin.scheme");

		//Set cursor image
		CEGUI::System::getSingletonPtr()->setDefaultMouseCursor(schemeName,"MouseArrow");
		CEGUI::MouseCursor::getSingletonPtr()->setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());
	}
	catch(Ogre::FileNotFoundException e)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage(e.getDescription(), Ogre::LML_CRITICAL);
	}
	catch(CEGUI::AlreadyExistsException e)
	{
		CEGUI::Logger::getSingletonPtr()->logEvent("Skin " + schemeName + " already loaded", CEGUI::Informative);
	}

	return;
}

void CeslitMenu::update(double timeSinceLastFrame)
{
	fade(timeSinceLastFrame);
}
void CeslitMenu::setAlpha( float alpha )
{
	menuBackground->setAlpha(alpha);
}
void CeslitMenu::createBackground(Ogre::String backgroundName , CEGUI::UVector2 position, CEGUI::UVector2 size)
{
	menuBackground = WindowManager::getSingletonPtr()->createWindow("DefaultWindow", backgroundName);
	//menuBackground = WindowManager::getSingletonPtr()->createWindow( schemeName + "/FrameWindow", backgroundName);
	menuBackground->setPosition(position);
	menuBackground->setSize(size);

/*
	menuBackground->setProperty( "Text", "Visitar escena");
  menuBackground->setProperty( "DragMovingEnabled",  "False");
  menuBackground->setProperty( "SizingEnabled",      "False");
	menuBackground->setProperty( "CloseButtonEnabled", "False");
*/

	menuBackground->setAlpha(0);
	rootGUIWindow->addChildWindow(menuBackground);

	return;
}

void CeslitMenu::createImage( Ogre::String fileName, CEGUI::UVector2 position, CEGUI::UVector2 size )
{
	/* StaticImage */
	CEGUI::ImagesetManager::getSingleton().createImagesetFromImageFile("ImageForStaticImage"+fileName, fileName );
	CEGUI::Window*   mImage		  = CEGUI::WindowManager::getSingleton().createWindow( schemeName + "/StaticImage", "BGpic" + fileName);
	Ogre::String		 setString = "set:ImageForStaticImage" + fileName +" image:full_image";
	mImage->setProperty("Image", setString); // "full_image" is a default name from CEGUIImageset::Imageset()
	mImage->setPosition( position );
	mImage->setSize( size );
	mImage->setAlpha(1.0);
	mImage->setZOrderingEnabled( false );
	mImage->setProperty("AlwaysOnTop", "False");
	mImage->moveToBack();
	mImage->setRiseOnClickEnabled( false );

	menuBackground->addChildWindow(mImage);

	return;
}

void CeslitMenu::showImage(int _id)
{
};

void CeslitMenu::createButton(Ogre::String name, Ogre::String text, CEGUI::UVector2 position,
															CEGUI::UVector2 size, SubscriberSlot subscriber, 
															Ogre::Real alpha = 0.7, Ogre::String tooltipText = NULL)
{
	PushButton* newButton = (PushButton*)WindowManager::getSingletonPtr()->createWindow(schemeName + "/Button", name);
	if (menuBackground == NULL)	throw Ogre::Exception( 0, "Background has not been defined", "CeslitMenu::createButton" );
	menuBackground->addChildWindow(newButton);

	newButton->setText(text);
	newButton->setTooltipText(tooltipText);
	newButton->setPosition(position);
	newButton->setSize(size);
	newButton->setAlpha(alpha);
	newButton->subscribeEvent(PushButton::EventClicked, subscriber);
	newButton->moveToFront();
  newButton->setProperty("AlwaysOnTop", "True");
	newButton->setRiseOnClickEnabled( true );
	return;
}

void CeslitMenu::createImageButton(		Ogre::String name, Ogre::String text, CEGUI::UVector2 position,
																			CEGUI::UVector2 size, SubscriberSlot subscriber, 
																			Ogre::Real alpha = 0.7, Ogre::String tooltipText = NULL)
{

	return;
}

void CeslitMenu::createSelectionScrollbar(Ogre::String name, Ogre::String text, CEGUI::UVector2 position,
							  CEGUI::UVector2 size, SubscriberSlot clickSubscriber, float documentSize, float step,
							  Ogre::Real alpha = 0.7, Ogre::String tooltipText = NULL, Ogre::String description = "")
{
	Window *newWindow = WindowManager::getSingletonPtr()->createWindow("DefaultWindow", name);
	if (menuBackground == NULL) throw Ogre::Exception(0, "Background has not been defined", "CeslitMenu::createScrollbar");
	menuBackground->addChildWindow(newWindow);

	newWindow->setPosition(position);
	newWindow->setTooltipText(tooltipText);
	newWindow->setSize(size);

	//Titlebar *title = (Titlebar*)WindowManager::getSingletonPtr()->createWindow(schemeName + "/Titlebar", (name + "Title"));
	PushButton* title = (PushButton*)WindowManager::getSingletonPtr()->createWindow(schemeName + "/Button", name + "Title");
	newWindow->addChildWindow(title);
	
	title->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	title->setSize(UVector2(UDim(1.0f, 0.0f), UDim(0.4f, 0.0f)));
	title->setText(text);
	title->setHorizontalAlignment(HA_CENTRE);
	title->disable();

	Scrollbar* newScrollbar = (Scrollbar*)WindowManager::getSingletonPtr()->createWindow(schemeName + "/HorizontalScrollbar", (name + "Scrollbar"));
	newWindow->addChildWindow(newScrollbar);

	newScrollbar->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.4f, 0.0f)));
	newScrollbar->setSize(UVector2(UDim(1.0f, 0.0f), UDim(0.2f, 0.0f)));
	newScrollbar->setDocumentSize(documentSize);
	newScrollbar->setPageSize(step);
	newScrollbar->setAlpha(alpha);
	newScrollbar->subscribeEvent(Scrollbar::EventMouseClick, clickSubscriber);

	//PushButton* button = static_cast<PushButton*>(WindowManager::getSingletonPtr()->getWindow(name + "Scrollbar" + Scrollbar::IncreaseButtonNameSuffix));
	//button->subscribeEvent(PushButton::EventClicked, positionSubscriber);
	//button = static_cast<PushButton*>(WindowManager::getSingletonPtr()->getWindow(name + "Scrollbar" + Scrollbar::DecreaseButtonNameSuffix));
	////button->subscribeEvent(PushButton::EventClicked, positionSubscriber);

	Window *descriptionText = WindowManager::getSingletonPtr()->createWindow(schemeName + "/StaticText", (name + "Description"));
	newWindow->addChildWindow(descriptionText);

	descriptionText->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.6f, 0.0f)));
	descriptionText->setSize(UVector2(UDim(0.3f, 0.0f), UDim(0.3f, 0.0f)));
	descriptionText->setText(description);
	descriptionText->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	descriptionText->setHorizontalAlignment(HA_CENTRE);

	return;
}

void CeslitMenu::show()
{
	menuBackground->show();

	return;
}

void CeslitMenu::hide()
{

	menuBackground->hide();

	return;
}

void CeslitMenu::processInput()
{
	menuMouse->capture();

	mouseMoved();

	if(menuMouse->getMouseState().buttonDown(OIS::MB_Left)) {
		System::getSingletonPtr()->injectMouseButtonDown(CEGUI::LeftButton);
	}
	else {
		System::getSingletonPtr()->injectMouseButtonUp(CEGUI::LeftButton);
	}

	return;
}

void CeslitMenu::mouseMoved()
{
	System *CEGUISystem = System::getSingletonPtr();
	Renderer *CEGUIRenderer = CEGUISystem->getRenderer();

	CEGUISystem->injectMouseMove(menuMouse->getMouseState().X.rel * CEGUIRenderer->getWidth() * cursorSpeed,
							menuMouse->getMouseState().Y.rel * CEGUIRenderer->getHeight() * cursorSpeed);
}

void CeslitMenu::startFadeIn(double duration )
{
	if( duration < 0 )
		duration = -duration;
	if( duration < 0.000001 )
		duration = 1.0;

	_alpha = 1.0;
	_total_dur = duration;
	_current_dur = duration;
	_fadeop = FADE_IN;
	menuBackground->show();
}

void CeslitMenu::startFadeOut(double duration )
{
	if( duration < 0 )
		duration = -duration;
	if( duration < 0.000001 )
		duration = 1.0;

	_alpha = 0.0;
	_total_dur = duration;
	_current_dur = 0.0;
	_fadeop = FADE_OUT;
	menuBackground->show();
}

void CeslitMenu::fade(double timeSinceLastFrame)
{
	if( _fadeop != FADE_NONE  )
	{
		// Set the _alpha value of the menu
		menuBackground->setAlpha( _alpha );
		// If fading in, decrease the _alpha until it reaches 0.0
		if( _fadeop == FADE_IN )
		{
			_current_dur -= timeSinceLastFrame;
			_alpha = _current_dur / _total_dur;
			if( _alpha < 0.0 )
			{
				menuBackground->hide();
				_fadeop = FADE_NONE;
				if( _inst )
					_inst->fadeInCallback();
			}
		}

		// If fading out, increase the _alpha until it reaches 1.0
		else if( _fadeop == FADE_OUT )
		{
			_current_dur += timeSinceLastFrame;
			_alpha = _current_dur / _total_dur;
			if( _alpha > 1.0 )
			{
				_fadeop = FADE_NONE;
				if( _inst )
					_inst->fadeOutCallback();
			}
		}
	}
}