#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/windowcontroller.hh"

using System::KeyCode;
using System::Event;
using System::Mutex;
using System::OpenGLContext;
using System::Window;
using Framework::KeyState;
using Framework::ReadingKeyboardState;
using Framework::ReadingMouseState;
using Framework::ReadingWindowState;
using Framework::WindowController;

WindowController::WindowController()
    : m_isWindowClosed(false)
{
    m_mutex = Mutex::Create();
    m_windowReady = Event::Create(NULL);
}

WindowController::~WindowController()
{
    if (m_windowReady != NULL)
        delete m_windowReady;
}

bool WindowController::isWindowClosed() const
{
    bool value;

    m_mutex->Lock();
    value = m_isWindowClosed;
    m_mutex->Unlock();

    return value;
}

ReadingKeyboardState *WindowController::GetKeyStateReader()
{
    return &m_keyboardState;
}

ReadingMouseState *WindowController::GetMouseReader()
{
    return &m_mouseState;
}

ReadingWindowState *WindowController::GetWindowReader()
{
    return &m_windowState;
}

void WindowController::OnWindowReady()
{
    m_windowReady->Trigger();
}

System::Event *WindowController::windowReady()
{
    return m_windowReady;
}

void WindowController::OnWindowClose()
{
    m_mutex->Lock();
    m_isWindowClosed = true;
    m_mutex->Unlock();
}

void WindowController::OnWindowResize(unsigned int width, unsigned int height)
{
    m_windowState.Resize(width, height);
}

void WindowController::OnKeyDown(KeyCode key)
{
    m_keyboardState.PressKey(key);

    for (std::list<KeyboardInputHandlerInterface*>::iterator iter = m_keyboardEventHandlers.begin(); iter != m_keyboardEventHandlers.end(); iter++)
    {
        (*iter)->PressKey(key);
    }
}

void WindowController::OnKeyUp(KeyCode key)
{
    m_keyboardState.UnpressKey(key);

    for (std::list<KeyboardInputHandlerInterface*>::iterator iter = m_keyboardEventHandlers.begin(); iter != m_keyboardEventHandlers.end(); iter++)
    {
        (*iter)->UnpressKey(key);
    }
}

void WindowController::OnMouseMove(int xPos, int yPos)
{
    m_mouseState.MouseMove(xPos, yPos);
}

void WindowController::OnMouseButtonDown(System::MouseButton button)
{
    m_mouseState.MouseButtonDown(button);
}

void WindowController::OnMouseButtonUp(System::MouseButton button)
{
    m_mouseState.MouseButtonUp(button);
}

void WindowController::OnMouseScroll(int scrollDelta)
{
    m_mouseState.MouseScroll(scrollDelta);
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

void WindowController::AddKeyboardEventHandler(KeyboardInputHandlerInterface *handler)
{
    m_keyboardEventHandlers.push_back(handler);
}

void WindowController::RemoveKeyboardEventHandler(KeyboardInputHandlerInterface *handler)
{
    m_keyboardEventHandlers.remove(handler);
}

WindowController::MouseState::MouseState()
{
    _scrollDelta = 0;
    m_mutex = Mutex::Create();
}

WindowController::MouseState::~MouseState()
{
    delete m_mutex;
}

int WindowController::MouseState::GetMouseX() const
{
    int pos;

    m_mutex->Lock();
    pos = _xPos;
    m_mutex->Unlock();

    return pos;
}

int WindowController::MouseState::GetMouseY() const
{
    int pos;

    m_mutex->Lock();
    pos = _yPos;
    m_mutex->Unlock();

    return pos;
}

KeyState WindowController::MouseState::GetMouseButtonState(System::MouseButton mouseButton)
{
    KeyState state;

    m_mutex->Lock();
    state = m_states[mouseButton];
    m_mutex->Unlock();

    return state;
}

int WindowController::MouseState::GetScrollDelta()
{
    int delta;

    m_mutex->Lock();
    delta = _scrollDelta;
    _scrollDelta = 0;
    m_mutex->Unlock();

    return delta;
}

void WindowController::MouseState::MouseMove(int xPos, int yPos)
{
    m_mutex->Lock();
    _xPos = xPos;
    _yPos = yPos;
    m_mutex->Unlock();
}

void WindowController::MouseState::MouseButtonDown(System::MouseButton mouseButton)
{
    m_mutex->Lock();
    m_states[mouseButton] = KeyState::Pressed;
    m_mutex->Unlock();
}

void WindowController::MouseState::MouseButtonUp(System::MouseButton mouseButton)
{
    m_mutex->Lock();
    m_states[mouseButton] = KeyState::Unpressed;
    m_mutex->Unlock();
}

void WindowController::MouseState::MouseScroll(int scrollDelta)
{
    m_mutex->Lock();
    _scrollDelta += scrollDelta;
    m_mutex->Unlock();
}

WindowController::WindowState::WindowState()
{
    m_mutex = Mutex::Create();
}

WindowController::WindowState::~WindowState()
{
    delete m_mutex;
}

void WindowController::WindowState::GetSize(unsigned int *width, unsigned int *height)
{
    m_mutex->Lock();
    *width = _width;
    *height = _height;
    m_mutex->Unlock();
}

void WindowController::WindowState::Resize(unsigned int width, unsigned int height)
{
    m_mutex->Lock();
    _width = width;
    _height = height;
    m_mutex->Unlock();
}
