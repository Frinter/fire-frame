#include "systemwindowcontroller.hh"

using System::KeyCode;
using System::Window;
using System::OpenGLContext;

SystemWindowController::SystemWindowController() : m_window(NULL)
{ }

SystemWindowController::~SystemWindowController()
{
	if (m_window != NULL)
		delete m_window;
}

void SystemWindowController::CreateWindow()
{
	m_window = Window::Create(this);
	m_window->DoMessageLoop();
}

void SystemWindowController::OnWindowReady()
{
	m_openGLContext = OpenGLContext::Create(m_window);
}

void SystemWindowController::OnKeyDown(KeyCode key)
{
	printf("Key Down\n");
}

void SystemWindowController::OnKeyUp(KeyCode key)
{
	printf("Key Up\n");
}
