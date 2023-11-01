#ifndef _GUIFunctor_H_
#define _GUIFunctor_H_

#include "stdio.h"


class GUIFunctor
{
public:

	typedef void (*GUICallback) (void *);


	// Constructor / Destructor
	GUIFunctor	 ();
	~GUIFunctor ();

	// Init / Release
	void  init          (GUICallback callback, void *operand = NULL);
	void  end           ();

	void setOperand (void *operand);

	void execute();

private:
	bool  m_bIsValid;

	void *m_operand;

	GUICallback call;
};

#endif // _GUIFunctor_H_
