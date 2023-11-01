#ifndef _SIGARMenu_H_
#define _SIGARMenu_H_

#include "GUIWindow.h"
#include "GUIImageButton.h"


/**
* SIGAR Menu
*/
class SIGARMenu : public GUIWindow
{
public:

	enum BUTTON_TYPE
	{
		CONTINUE,
		BACK
	};

	// Constructor / Destructor
	SIGARMenu	();
	~SIGARMenu	();

	// Init / Release / Update
	void	init	(Cing::String name, Cing::String backgroundImageFile, GUIFunctor::GUICallback continueDo, GUIFunctor::GUICallback backDo);
	//void	update	();
	void	end		();

	void	executeContinueDo();
	void	executeBackDo();

	void	setLastButtonPressed(BUTTON_TYPE button);

	void	setContinueDo(GUIFunctor::GUICallback);
	void	setBackDo(GUIFunctor::GUICallback);

	BUTTON_TYPE	getLastButtonPressed();

private:
	GUIImageButton	m_continueButton,
					m_backButton;

	bool			m_isValid;

	BUTTON_TYPE		m_lastButtonPressed;

	GUIFunctor::GUICallback		continueDo,
								backDo;
};

#endif // _SIGARMenu_H_
