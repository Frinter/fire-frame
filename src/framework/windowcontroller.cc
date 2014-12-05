#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/windowcontroller.hh"

using System::KeyCode;
using System::Mutex;
using System::OpenGLContext;
using System::Window;
using Framework::KeyState;
using Framework::ReadingKeyboardState;
using Framework::WindowController;

WindowController::WindowController() : m_window(NULL)
{ }

WindowController::~WindowController()
{
	if (m_window != NULL)
		delete m_window;
}

void WindowController::CreateWindow()
{
	m_window = Window::Create(this);
	m_window->DoMessageLoop();
}

ReadingKeyboardState *WindowController::GetKeyStateReader()
{
	return &m_keyboardState;
}

void WindowController::OnWindowReady()
{
	m_openGLContext = OpenGLContext::Create(m_window);
}

void WindowController::OnKeyDown(KeyCode key)
{
	m_keyboardState.PressKey(key);
}

void WindowController::OnKeyUp(KeyCode key)
{
	m_keyboardState.UnpressKey(key);
}

WindowController::KeyboardState::KeyboardState()
{
	m_mutex = Mutex::Create();
}

WindowController::KeyboardState::~KeyboardState()
{
	delete m_mutex;
}

KeyState WindowController::KeyboardState::GetKeyState(KeyCode key)
{
	KeyState state;

	m_mutex->Lock();
	state = m_states[key];
	m_mutex->Unlock();

	return state;
}

void WindowController::KeyboardState::PressKey(KeyCode key)
{
	m_mutex->Lock();
	m_states[key] = Pressed;
	m_mutex->Unlock();
}

void WindowController::KeyboardState::UnpressKey(KeyCode key)
{
	m_mutex->Lock();
	m_states[key] = Unpressed;
	m_mutex->Unlock();
}
