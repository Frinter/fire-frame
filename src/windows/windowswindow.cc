#include <stdexcept>
#include <unordered_map>

#include "windowsx.h"

#include "windowswindow.hh"
#include "windowsopenglcontext.hh"
#include "system/mousebutton.hh"
#include "system/keycode.hh"

static char appName[] = "Test Application";

using System::KeyCode;

class KeyMap : public std::unordered_map<WPARAM, System::KeyCode>
{
public:
    KeyMap() : std::unordered_map<WPARAM, System::KeyCode>() {
        (*this)[VK_PRIOR]   = KeyCode::KeyPageUp;
        (*this)[VK_NEXT]    = KeyCode::KeyPageDown;
        (*this)[VK_END]     = KeyCode::KeyEnd;
        (*this)[VK_HOME]    = KeyCode::KeyHome;
        (*this)[VK_INSERT]  = KeyCode::KeyInsert;
        (*this)[VK_DELETE]  = KeyCode::KeyDelete;

        (*this)[VK_RIGHT]   = KeyCode::KeyRightArrow;
        (*this)[VK_DOWN]    = KeyCode::KeyDownArrow;
        (*this)[VK_LEFT]    = KeyCode::KeyLeftArrow;
        (*this)[VK_UP]      = KeyCode::KeyUpArrow;

        (*this)[VK_ESCAPE]  = KeyCode::KeyEscape;
        (*this)[VK_TAB]     = KeyCode::KeyTab;
        (*this)[VK_SHIFT]   = KeyCode::KeyShift;
        (*this)[VK_MENU]    = KeyCode::KeyAlt;
        (*this)[VK_CONTROL] = KeyCode::KeyControl;
        (*this)[VK_CAPITAL] = KeyCode::KeyCapsLock;

        (*this)[VK_RCONTROL] = KeyCode::KeyRightControl;
        (*this)[VK_LCONTROL] = KeyCode::KeyLeftControl;
        (*this)[VK_RSHIFT]   = KeyCode::KeyRightShift;
        (*this)[VK_LSHIFT]   = KeyCode::KeyLeftShift;

        (*this)[VK_BACK]    = KeyCode::KeyBackspace;
        (*this)[VK_RETURN]  = KeyCode::KeyEnter;
        (*this)[VK_SPACE]   = KeyCode::KeySpace;

        (*this)[0x30]       = KeyCode::Key0;
        (*this)[0x31]       = KeyCode::Key1;
        (*this)[0x32]       = KeyCode::Key2;
        (*this)[0x33]       = KeyCode::Key3;
        (*this)[0x34]       = KeyCode::Key4;
        (*this)[0x35]       = KeyCode::Key5;
        (*this)[0x36]       = KeyCode::Key6;
        (*this)[0x37]       = KeyCode::Key7;
        (*this)[0x38]       = KeyCode::Key8;
        (*this)[0x39]       = KeyCode::Key9;

        (*this)[VK_NUMPAD0] = KeyCode::KeyNumpad0;
        (*this)[VK_NUMPAD1] = KeyCode::KeyNumpad1;
        (*this)[VK_NUMPAD2] = KeyCode::KeyNumpad2;
        (*this)[VK_NUMPAD3] = KeyCode::KeyNumpad3;
        (*this)[VK_NUMPAD4] = KeyCode::KeyNumpad4;
        (*this)[VK_NUMPAD5] = KeyCode::KeyNumpad5;
        (*this)[VK_NUMPAD6] = KeyCode::KeyNumpad6;
        (*this)[VK_NUMPAD7] = KeyCode::KeyNumpad7;
        (*this)[VK_NUMPAD8] = KeyCode::KeyNumpad8;
        (*this)[VK_NUMPAD9] = KeyCode::KeyNumpad9;

        (*this)[VK_MULTIPLY] = KeyCode::KeyMultiply;
        (*this)[VK_ADD]      = KeyCode::KeyPlus;
        (*this)[VK_SUBTRACT] = KeyCode::KeyMinus;
        (*this)[VK_DIVIDE]   = KeyCode::KeyDivide;
        (*this)[VK_DECIMAL]  = KeyCode::KeyDecimal;

        (*this)[0x41]       = KeyCode::KeyA;
        (*this)[0x42]       = KeyCode::KeyB;
        (*this)[0x43]       = KeyCode::KeyC;
        (*this)[0x44]       = KeyCode::KeyD;
        (*this)[0x45]       = KeyCode::KeyE;
        (*this)[0x46]       = KeyCode::KeyF;
        (*this)[0x47]       = KeyCode::KeyG;
        (*this)[0x48]       = KeyCode::KeyH;
        (*this)[0x49]       = KeyCode::KeyI;
        (*this)[0x4A]       = KeyCode::KeyJ;
        (*this)[0x4B]       = KeyCode::KeyK;
        (*this)[0x4C]       = KeyCode::KeyL;
        (*this)[0x4D]       = KeyCode::KeyM;
        (*this)[0x4E]       = KeyCode::KeyN;
        (*this)[0x4F]       = KeyCode::KeyO;
        (*this)[0x50]       = KeyCode::KeyP;
        (*this)[0x51]       = KeyCode::KeyQ;
        (*this)[0x52]       = KeyCode::KeyR;
        (*this)[0x53]       = KeyCode::KeyS;
        (*this)[0x54]       = KeyCode::KeyT;
        (*this)[0x55]       = KeyCode::KeyU;
        (*this)[0x56]       = KeyCode::KeyV;
        (*this)[0x57]       = KeyCode::KeyW;
        (*this)[0x59]       = KeyCode::KeyY;
        (*this)[0x5A]       = KeyCode::KeyZ;
    }
};

static KeyMap keyMap;
static std::unordered_map<HWND, WindowsWindow*> windowMap;

WindowsWindow::WindowsWindow(const char *windowName, Framework::ISystemWindowController *controller, HINSTANCE processInstance, int commandShow)
    : m_controller(controller), m_openGLContext(NULL)
{
    m_windowHandle = WindowsWindow::makeWindow(windowName, processInstance, commandShow);
    windowMap[m_windowHandle] = this;

    ShowWindow(m_windowHandle, commandShow);
    UpdateWindow(m_windowHandle);

    unsigned int windowWidth, windowHeight;
    GetWindowSize(&windowWidth, &windowHeight);
    m_controller->OnWindowResize(windowWidth, windowHeight);
}

HWND WindowsWindow::makeWindow(const char *windowName, HINSTANCE processInstance, int commandShow)
{
    WNDCLASSEX windowClass;

    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.style         = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc   = WndProc;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = processInstance;
    windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    windowClass.lpszClassName = appName;
    windowClass.lpszMenuName  = NULL;

    RegisterClassEx(&windowClass);

    return CreateWindow(appName, windowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, processInstance, NULL);
}

WindowsWindow::~WindowsWindow()
{
    if (m_openGLContext != NULL)
        delete m_openGLContext;

    windowMap[m_windowHandle] = NULL;
}

int WindowsWindow::DoMessageLoop()
{
    MSG message;

    Ready();

    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}

System::OpenGLContext *WindowsWindow::getOrCreateOpenGLContext()
{
    if (m_openGLContext == NULL)
        m_openGLContext = new WindowsOpenGLContext(m_windowHandle);

    return m_openGLContext;
}

void WindowsWindow::GetWindowSize(unsigned int *width, unsigned int *height)
{
    RECT rect;

    GetClientRect(m_windowHandle, &rect);

    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
}

bool WindowsWindow::SetMousePosition(unsigned int posX, unsigned int posY)
{
    POINT point;

    point.x = posX;
    point.y = posY;

    bool success = ClientToScreen(m_windowHandle, &point) ? SetCursorPos(point.x, point.y) : false;
    if (success)
        m_controller->OnMouseMove(point.x, point.y);

    return success;
}

void WindowsWindow::Destroy()
{
    SendNotifyMessage(m_windowHandle, WM_DESTROY, 0, 0);
}

void WindowsWindow::Close()
{
    SendNotifyMessage(m_windowHandle, WM_CLOSE, 0, 0);
}

void WindowsWindow::Ready()
{
    m_controller->OnWindowReady();
}

void WindowsWindow::onClose()
{
    m_controller->OnWindowClose();
}

void WindowsWindow::KeyDown(WPARAM key)
{
    m_controller->OnKeyDown(keyMap[key]);
}

void WindowsWindow::WindowResize(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnWindowResize(LOWORD(lParam), HIWORD(lParam));
}

void WindowsWindow::KeyUp(WPARAM key)
{
    m_controller->OnKeyUp(keyMap[key]);
}

void WindowsWindow::MouseMove(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
}

void WindowsWindow::LeftMouseButtonDown(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseButtonDown(System::MouseButton::ButtonOne);
}

void WindowsWindow::RightMouseButtonDown(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseButtonDown(System::MouseButton::ButtonTwo);
}

void WindowsWindow::LeftMouseButtonUp(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseButtonUp(System::MouseButton::ButtonOne);
}

void WindowsWindow::RightMouseButtonUp(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseButtonUp(System::MouseButton::ButtonTwo);
}

void WindowsWindow::MouseScrollWheel(WPARAM wParam, LPARAM lParam)
{
    m_controller->OnMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
}

LRESULT CALLBACK WindowsWindow::WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch(message)
    {
    case WM_KEYDOWN:
        windowMap[windowHandle]->KeyDown(wparam);
        return 0;

    case WM_KEYUP:
        windowMap[windowHandle]->KeyUp(wparam);
        return 0;

    case WM_MOUSEMOVE:
        windowMap[windowHandle]->MouseMove(wparam, lparam);
        return 0;

    case WM_LBUTTONDOWN:
        windowMap[windowHandle]->LeftMouseButtonDown(wparam, lparam);
        return 0;

    case WM_LBUTTONUP:
        windowMap[windowHandle]->LeftMouseButtonUp(wparam, lparam);
        return 0;

    case WM_RBUTTONDOWN:
        windowMap[windowHandle]->RightMouseButtonDown(wparam, lparam);
        return 0;

    case WM_RBUTTONUP:
        windowMap[windowHandle]->RightMouseButtonUp(wparam, lparam);
        return 0;

    case WM_MOUSEWHEEL:
        windowMap[windowHandle]->MouseScrollWheel(wparam, lparam);
        return 0;

    case WM_SIZE:
        windowMap[windowHandle]->WindowResize(wparam, lparam);
        return 0;

    case WM_CLOSE:
        windowMap[windowHandle]->onClose();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        windowMap[windowHandle] = NULL;
        return 0;

    default:
        return DefWindowProc(windowHandle, message, wparam, lparam);
    }
}
