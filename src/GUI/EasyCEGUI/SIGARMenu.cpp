#include "SIGARMenu.h"

#include "stdarg.h"

static double FADE_OUT_TIME = 1.0;

void continueButtonDo(void *param)
{
	SIGARMenu* menu = ((SIGARMenu*)param);

	menu->setLastButtonPressed(SIGARMenu::CONTINUE);
	menu->startFadeOut(FADE_OUT_TIME);
}

void backButtonDo(void *param)
{
	SIGARMenu *menu = ((SIGARMenu*)param);

	menu->setLastButtonPressed(SIGARMenu::BACK);
	menu->startFadeOut(FADE_OUT_TIME);
}

void fadeOutEnded(void *param)
{
	SIGARMenu *menu = ((SIGARMenu*)param);
	SIGARMenu::BUTTON_TYPE pressed = menu->getLastButtonPressed();

	if (pressed == SIGARMenu::CONTINUE) menu->executeContinueDo();
	else if (pressed == SIGARMenu::BACK) menu->executeBackDo();
}

SIGARMenu::SIGARMenu() : continueDo(NULL), backDo(NULL)
{
}

SIGARMenu::~SIGARMenu()
{
}

void SIGARMenu::init(Cing::String name, Cing::String backgroundImageFile, GUIFunctor::GUICallback continueCallback, GUIFunctor::GUICallback backCallback)
{
	((GUIWindow*)this)->init(name + "/Main");

	addBackground(backgroundImageFile);

	m_continueButton.init("GUI_Resources/Buttons/botonContinuar", name + "/ContinueButton" , -36, 340, 100, 100 );
	addChild(m_continueButton);
	m_continueButton.setPushCallback(continueButtonDo, this);

	m_backButton.init("GUI_Resources/Buttons/botonVolver" , name + "/BackButton" , 580, 340, 100, 100 );
	addChild(m_backButton);
	m_backButton.setPushCallback(backButtonDo, this);

	setFadeOutCallback(fadeOutEnded, this);

	this->continueDo = continueCallback;
	this->backDo = backCallback;
}

void SIGARMenu::executeContinueDo()
{
	if (continueDo) continueDo(NULL);
}

void SIGARMenu::executeBackDo()
{
	if (backDo) backDo(NULL);
}

void SIGARMenu::setLastButtonPressed(BUTTON_TYPE button)
{
	m_lastButtonPressed = button;
}

void SIGARMenu::setContinueDo(GUIFunctor::GUICallback callback)
{
	continueDo = callback;
}

void SIGARMenu::setBackDo(GUIFunctor::GUICallback callback)
{
	backDo = callback;
}

SIGARMenu::BUTTON_TYPE SIGARMenu::getLastButtonPressed()
{
	return m_lastButtonPressed;
}
