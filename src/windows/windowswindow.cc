#include <stdexcept>
#include <unordered_map>

#include "windowswindow.hh"
#include "windowsopenglcontext.hh"

#define FILE_EXIT_OPTION 9001
static char appName[] = "Test Application";

class KeyMap : public std::unordered_map<WPARAM, System::KeyCode> {
public:
	KeyMap() : std::unordered_map<WPARAM, System::KeyCode>() {
		(*this)[VK_PRIOR]   = System::KeyPageUp;
		(*this)[VK_NEXT]    = System::KeyPageDown;
		(*this)[VK_END]     = System::KeyEnd;
		(*this)[VK_HOME]    = System::KeyHome;
		(*this)[VK_INSERT]  = System::KeyInsert;
		(*this)[VK_DELETE]  = System::KeyDelete;

		(*this)[VK_RIGHT]   = System::KeyRightArrow;
		(*this)[VK_DOWN]    = System::KeyDownArrow;
		(*this)[VK_LEFT]    = System::KeyLeftArrow;
		(*this)[VK_UP]      = System::KeyUpArrow;

		(*this)[VK_ESCAPE]  = System::KeyEscape;
		(*this)[VK_TAB]     = System::KeyTab;
		(*this)[VK_SHIFT]   = System::KeyShift;
		(*this)[VK_MENU]    = System::KeyAlt;
		(*this)[VK_CONTROL] = System::KeyControl;
		(*this)[VK_CAPITAL] = System::KeyCapsLock;

		(*this)[VK_RCONTROL] = System::KeyRightControl;
		(*this)[VK_LCONTROL] = System::KeyLeftControl;
		(*this)[VK_RSHIFT]   = System::KeyRightShift;
		(*this)[VK_LSHIFT]   = System::KeyLeftShift;

		(*this)[VK_BACK]    = System::KeyBackspace;
		(*this)[VK_RETURN]  = System::KeyEnter;
		(*this)[VK_SPACE]   = System::KeySpace;

		(*this)[0x30]       = System::Key0;
		(*this)[0x31]       = System::Key1;
		(*this)[0x32]       = System::Key2;
		(*this)[0x33]       = System::Key3;
		(*this)[0x34]       = System::Key4;
		(*this)[0x35]       = System::Key5;
		(*this)[0x36]       = System::Key6;
		(*this)[0x37]       = System::Key7;
		(*this)[0x38]       = System::Key8;
		(*this)[0x39]       = System::Key9;

		(*this)[VK_NUMPAD0] = System::KeyNumpad0;
		(*this)[VK_NUMPAD1] = System::KeyNumpad1;
		(*this)[VK_NUMPAD2] = System::KeyNumpad2;
		(*this)[VK_NUMPAD3] = System::KeyNumpad3;
		(*this)[VK_NUMPAD4] = System::KeyNumpad4;
		(*this)[VK_NUMPAD5] = System::KeyNumpad5;
		(*this)[VK_NUMPAD6] = System::KeyNumpad6;
		(*this)[VK_NUMPAD7] = System::KeyNumpad7;
		(*this)[VK_NUMPAD8] = System::KeyNumpad8;
		(*this)[VK_NUMPAD9] = System::KeyNumpad9;

		(*this)[VK_MULTIPLY] = System::KeyMultiply;
		(*this)[VK_ADD]      = System::KeyPlus;
		(*this)[VK_SUBTRACT] = System::KeyMinus;
		(*this)[VK_DIVIDE]   = System::KeyDivide;
		(*this)[VK_DECIMAL]  = System::KeyDecimal;

		(*this)[0x41]       = System::KeyA;
		(*this)[0x42]       = System::KeyB;
		(*this)[0x43]       = System::KeyC;
		(*this)[0x44]       = System::KeyD;
		(*this)[0x45]       = System::KeyE;
		(*this)[0x46]       = System::KeyF;
		(*this)[0x47]       = System::KeyG;
		(*this)[0x48]       = System::KeyH;
		(*this)[0x49]       = System::KeyI;
		(*this)[0x4A]       = System::KeyJ;
		(*this)[0x4B]       = System::KeyK;
		(*this)[0x4C]       = System::KeyL;
		(*this)[0x4D]       = System::KeyM;
		(*this)[0x4E]       = System::KeyN;
		(*this)[0x4F]       = System::KeyO;
		(*this)[0x50]       = System::KeyP;
		(*this)[0x51]       = System::KeyQ;
		(*this)[0x52]       = System::KeyR;
		(*this)[0x53]       = System::KeyS;
		(*this)[0x54]       = System::KeyT;
		(*this)[0x55]       = System::KeyU;
		(*this)[0x56]       = System::KeyV;
		(*this)[0x57]       = System::KeyW;
		(*this)[0x59]       = System::KeyY;
		(*this)[0x5A]       = System::KeyZ;
	}
};

static KeyMap keyMap;
static std::unordered_map<HWND, WindowsWindow*> windowMap;

WindowsWindow::WindowsWindow(System::WindowController *controller, HINSTANCE processInstance, int commandShow)
	: m_controller(controller), m_openGLContext(NULL)
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

	m_windowHandle = CreateWindow(appName, "Fire Frame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, processInstance, NULL);

	windowMap[m_windowHandle] = this;

	ShowWindow(m_windowHandle, commandShow);
	UpdateWindow(m_windowHandle);
}

WindowsWindow::~WindowsWindow() {
	windowMap[m_windowHandle] = NULL;
}

int WindowsWindow::DoMessageLoop() {
	MSG message;

	Ready();

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

void WindowsWindow::Ready()
{
	m_controller->OnWindowReady();
}

void WindowsWindow::KeyDown(WPARAM key)
{
	m_controller->OnKeyDown(keyMap[key]);
}

void WindowsWindow::KeyUp(WPARAM key)
{
	m_controller->OnKeyUp(keyMap[key]);
}

LRESULT CALLBACK WindowsWindow::WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam) {
	PAINTSTRUCT ps;
	HDC hdc;
	HMENU fileMenuHandle, mainMenuHandle;
			
	switch(message)
	{
	case WM_CREATE:
		mainMenuHandle = CreateMenu();
		fileMenuHandle = CreatePopupMenu();

		AppendMenu(fileMenuHandle, MF_STRING, FILE_EXIT_OPTION, "Exit");

		AppendMenu(mainMenuHandle, MF_STRING | MF_POPUP, (UINT)fileMenuHandle, "File");

		SetMenu(windowHandle, mainMenuHandle);

		return 0;

	case WM_KEYDOWN:
		windowMap[windowHandle]->KeyDown(wparam);
		return 0;

	case WM_KEYUP:
		windowMap[windowHandle]->KeyUp(wparam);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case FILE_EXIT_OPTION:
			PostMessage(windowHandle, WM_CLOSE, 0, 0);
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(windowHandle, &ps);
		TextOut(hdc, 100, 100, "Hello world", 11);
		EndPaint(windowHandle, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wparam, lparam);
}
