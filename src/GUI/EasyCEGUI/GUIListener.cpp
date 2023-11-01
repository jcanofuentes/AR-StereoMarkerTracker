#include "GUIListener.h"

/**
 * @internal
 * @brief Constructor 
 */
GUIListener::GUIListener() : pushed (NULL), fadeInEnded(NULL), fadeOutEnded(NULL)
{};

/**
 * @brief Destructor
 *
 */
GUIListener::~GUIListener()
{
	// Release resources
	end();
};

/**
 * @brief Destructor
 *
 */
void  GUIListener::init()
{
	// TODO: Create resources
	m_bIsValid = true;
};

void  GUIListener::end()
{
	// TODO: Release resources
	m_bIsValid = false;
};

bool GUIListener::executePushed(const CEGUI::EventArgs &)
{
	pushed->execute();

	return true;
}

void GUIListener::executeFadeInEnded()
{
	if (fadeInEnded) fadeInEnded->execute();
}

void GUIListener::executeFadeOutEnded()
{
	if (fadeOutEnded) fadeOutEnded->execute();
}

void GUIListener::setPushCallback(GUIFunctor::GUICallback callback, void *operand)
{
	if (pushed) delete (pushed);

	pushed = new GUIFunctor();
	pushed->init(callback, operand);
}

void GUIListener::setFadeInCallback(GUIFunctor::GUICallback callback, void *operand)
{
	if (fadeInEnded) delete (fadeInEnded);

	fadeInEnded = new GUIFunctor();
	fadeInEnded->init(callback, operand);
}

void GUIListener::setFadeOutCallback(GUIFunctor::GUICallback callback, void *operand)
{
	if (fadeOutEnded) delete (fadeOutEnded);
	
	fadeOutEnded = new GUIFunctor();
	fadeOutEnded->init(callback, operand);
}
