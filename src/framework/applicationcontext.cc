#include "framework/applicationcontext.hh"

using namespace Framework;

ApplicationContext::ApplicationContext()
	: m_isClosing(false), m_destroyWindowFlag(false)
{
}

void ApplicationContext::Close()
{
	m_isClosing = true;
}

void ApplicationContext::SignalWindowDestruction()
{
	m_destroyWindowFlag = true;
}

bool ApplicationContext::IsClosing() const
{
	return m_isClosing;
}

bool ApplicationContext::ShouldDestroyWindow() const
{
	return m_isClosing;
}
