#ifndef _GUIListener_H_
#define _GUIListener_H_


#include "GUIFunctor.h"

// CEGUI
#include "Ogre3d/include/cegui/CEGUIEventArgs.h"

/**
 * Clase para testear el funcionamiento de los eventos recibidos
 * desde cualquier elemento del GUI. 
 * xComentar: ¿Lo hacemos una clase abstracta de la que deriven?
 * O instanciamos este objeto con acceso a un "padre"...? Detallar esto*
 * 
 * TODO: Revisar eventos existentes y hacer listener para todos ellos
 * Ref:  http://www.cegui.org.uk/wiki/index.php/Identifying_Multiple_Event_Sources_From_A_Single_Callback
 */


class GUIListener
{
public:

	// Constructor / Destructor
	GUIListener	 ();
	~GUIListener ();

	// Init / Release
	void  init          ();
	void  end           ();

	// Mouse and keyboard events
	//void mouseMoved   (){};
	//void mousePressed (){};
	//void mouseReleased(){};
	//void keyPressed   (){};
	//void buttonPressed() {};
	//void buttonReleased() {};
	void setPushCallback (GUIFunctor::GUICallback callback, void *operand);
	void setFadeInCallback (GUIFunctor::GUICallback callback, void *operand);
	void setFadeOutCallback (GUIFunctor::GUICallback callback, void *operand);

	void setPushFunctor(GUIFunctor *functor);
	void setFadeInFunctor(GUIFunctor *functor);
	void setFadeOutFunctor(GUIFunctor *functor);

	// Begin/End transitions events
	//void beginFadeOp( FadeOperation fadeOp ){};
	//void endFadeOp  ( FadeOperation fadeOp ){};

	bool executePushed(const CEGUI::EventArgs&);
	void executeFadeInEnded();
	void executeFadeOutEnded();

private:
	bool  m_bIsValid;

protected:
	GUIFunctor *pushed;
	GUIFunctor *fadeInEnded;
	GUIFunctor *fadeOutEnded;
};

#endif // _GUIListener_H_
