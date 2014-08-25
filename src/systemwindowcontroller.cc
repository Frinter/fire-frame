#include "systemwindowcontroller.hh"

using System::KeyCode;
using System::KeyState;
using System::Window;
using System::OpenGLContext;
using System::SystemWindowController;

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

SystemWindowController::ReadingKeyboardState *SystemWindowController::GetKeyStateReader()
{
	return &m_keyboardState;
}

void SystemWindowController::OnWindowReady()
{
	m_openGLContext = OpenGLContext::Create(m_window);
}

void SystemWindowController::OnKeyDown(KeyCode key)
{
	m_keyboardState.PressKey(key);
}

void SystemWindowController::OnKeyUp(KeyCode key)
{
	m_keyboardState.UnpressKey(key);
}

SystemWindowController::KeyboardState::KeyboardState()
{
	m_mutex = Mutex::Create();
}

SystemWindowController::KeyboardState::~KeyboardState()
{
	delete m_mutex;
}

KeyState SystemWindowController::KeyboardState::GetKeyState(KeyCode key)
{
	KeyState state;

	m_mutex->Lock();
	state = Unpressed;
	m_mutex->Unlock();

	return state;
}

void SystemWindowController::KeyboardState::PressKey(KeyCode key)
{
	m_mutex->Lock();
	printf("Key pressed\n");
	m_mutex->Unlock();
}

void SystemWindowController::KeyboardState::UnpressKey(KeyCode key)
{
	m_mutex->Lock();
	printf("Key unpressed\n");
	m_mutex->Unlock();
}
