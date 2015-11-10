#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/windowcontroller.hh"

using System::KeyCode;
using System::Mutex;
using System::OpenGLContext;
using System::Window;
using Framework::KeyState;
using Framework::ReadingKeyboardState;
using Framework::ReadingMouseState;
using Framework::WindowController;

WindowController::WindowController(ApplicationContext *applicationContext)
    : m_window(NULL), m_openGLContext(NULL), m_shouldDestroyWindow(false), m_applicationContext(applicationContext)
{ }

WindowController::~WindowController()
{
    if (m_openGLContext != NULL)
        delete m_openGLContext;
    if (m_window != NULL)
        delete m_window;
}

void WindowController::CreateClientWindow()
{
    m_window = Window::Create(m_applicationContext, this);
    m_window->DoMessageLoop();
}

void WindowController::CreateContext()
{
    m_openGLContext = OpenGLContext::Create(m_window);
}

void WindowController::SwapBuffers()
{
    m_openGLContext->SwapBuffers();
}

ReadingKeyboardState *WindowController::GetKeyStateReader()
{
    return &m_keyboardState;
}

ReadingMouseState *WindowController::GetMouseReader()
{
    return &m_mouseState;
}

void WindowController::OnWindowReady()
{
    m_applicationContext->WindowReady()->Trigger();
}

void WindowController::OnWindowClose()
{
    m_applicationContext->Close();
}

void WindowController::OnKeyDown(KeyCode key)
{
    m_keyboardState.PressKey(key);

    for (std::list<IWritingKeyboardState*>::iterator iter = m_keyboardEventHandlers.begin(); iter != m_keyboardEventHandlers.end(); iter++)
    {
        (*iter)->PressKey(key);
    }
}

void WindowController::OnKeyUp(KeyCode key)
{
    m_keyboardState.UnpressKey(key);

    for (std::list<IWritingKeyboardState*>::iterator iter = m_keyboardEventHandlers.begin(); iter != m_keyboardEventHandlers.end(); iter++)
    {
        (*iter)->UnpressKey(key);
    }
}

void WindowController::OnMouseMove(int xPos, int yPos)
{
    m_mouseState.MouseMove(xPos, yPos);
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
    m_states[key] = KeyState::Pressed;
    m_mutex->Unlock();
}

void WindowController::KeyboardState::UnpressKey(KeyCode key)
{
    m_mutex->Lock();
    m_states[key] = KeyState::Unpressed;
    m_mutex->Unlock();
}

void WindowController::AddKeyboardEventHandler(IWritingKeyboardState *handler)
{
    m_keyboardEventHandlers.push_back(handler);
}

void WindowController::RemoveKeyboardEventHandler(IWritingKeyboardState *handler)
{
    m_keyboardEventHandlers.remove(handler);
}

WindowController::MouseState::MouseState()
{
    m_mutex = Mutex::Create();
}

WindowController::MouseState::~MouseState()
{
    delete m_mutex;
}

int WindowController::MouseState::GetMouseX() const
{
    return _xPos;
}

int WindowController::MouseState::GetMouseY() const
{
    return _yPos;
}

void WindowController::MouseState::MouseMove(int xPos, int yPos)
{
    _xPos = xPos;
    _yPos = yPos;
}
