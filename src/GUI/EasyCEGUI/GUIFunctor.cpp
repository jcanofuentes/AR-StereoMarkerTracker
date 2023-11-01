#include "GUIFunctor.h"

// Common
#include "common/LogManager.h"


GUIFunctor::GUIFunctor() : m_operand(NULL), call(NULL) { }

GUIFunctor::~GUIFunctor()
{
	end();
}

void GUIFunctor::init(GUICallback callback, void *operand)
{
	call = callback;
	m_operand = operand;

	m_bIsValid = true;
}

void GUIFunctor::end()
{
	if (m_operand)	delete (m_operand);
}

void GUIFunctor::setOperand(void *operand)
{
	m_operand = operand;
}

void GUIFunctor::execute()
{
	if (!m_bIsValid)
	{
		using namespace Cing;
		Cing::LOG_ERROR( "The functor has not been properly initialized." );
		return;
	}

	if (call) call(m_operand);
}
