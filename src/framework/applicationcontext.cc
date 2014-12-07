#include "framework/applicationcontext.hh"

using namespace Framework;

ApplicationContext::ApplicationContext()
	: m_isClosing(false)
{
}

void ApplicationContext::Close()
{
	m_isClosing = true;
}

bool ApplicationContext::IsClosing() const
{
	return m_isClosing;
}
