#include "framework/applicationcontext.hh"

using namespace Framework;

ApplicationContext::ApplicationContext()
	: m_isClosing(false), m_destroyWindowFlag(false), m_windowReady(System::Event::Create("WindowReady")), m_graphicsThreadQuit(System::Event::Create("GraphicsThreadQuit"))
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

System::Event *ApplicationContext::WindowReady() const
{
	return m_windowReady;
}

System::Event *ApplicationContext::GraphicsThreadQuit() const
{
	return m_graphicsThreadQuit;
}
