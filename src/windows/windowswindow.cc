#include <stdexcept>
#include <unordered_map>

#include "windowswindow.h"
#include "windowsopenglcontext.h"

#define FILE_EXIT_OPTION 9001
static char appName[] = "Test Application";

class KeyMap : public std::unordered_map<WPARAM, System::KeyCode> {
public:
	KeyMap() : std::unordered_map<WPARAM, System::KeyCode>() {
		(*this)[VK_ESCAPE] = System::KeyEscape;
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
