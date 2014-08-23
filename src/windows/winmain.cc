#include <map>
#include <stdexcept>
#include <windows.h>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/wglew.h>

#include "../window.h"

#define FILE_EXIT_OPTION 9001

extern int applicationMain();

static char appName[] = "Test Application";

class WindowsWindow;

HINSTANCE windows_instance;
HINSTANCE windows_parentInstance;
LPSTR windows_arguments;
int windows_commandShow;
std::map<HWND, WindowsWindow*> windowMap;

class WindowsWindow : public System::Window
{
public:
	WindowsWindow() : m_openGLContext(NULL) {
		WNDCLASSEX windowClass;

		windowClass.cbSize        = sizeof(WNDCLASSEX);
		windowClass.style         = CS_VREDRAW | CS_HREDRAW;
		windowClass.lpfnWndProc   = WndProc;
		windowClass.cbClsExtra    = 0;
		windowClass.cbWndExtra    = 0;
		windowClass.hInstance     = windows_instance;
		windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
		windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
		windowClass.lpszClassName = appName;
		windowClass.lpszMenuName  = NULL;

		RegisterClassEx(&windowClass);

		m_windowHandle = CreateWindow(appName, "Fire Frame", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, windows_instance, NULL);

		ShowWindow(m_windowHandle, windows_commandShow);
		UpdateWindow(m_windowHandle);

		windowMap[m_windowHandle] = this;
	}

	virtual ~WindowsWindow() {
		windowMap[m_windowHandle] = NULL;
	}

	virtual int DoMessageLoop() {
		MSG message;

		while (GetMessage(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		return message.wParam;
	}

	void CreateOpenGLContext() {
		if (m_openGLContext != NULL)
			return;

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		HDC deviceContext = GetDC(m_windowHandle);
		int pixelFormatIndex = ChoosePixelFormat(deviceContext, &pfd);
		SetPixelFormat(deviceContext, pixelFormatIndex, &pfd);

		HGLRC tempContext = wglCreateContext(deviceContext);
		wglMakeCurrent(deviceContext, tempContext);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
			throw new std::runtime_error("Failed to initialize GLEW");

		if (wglewIsSupported("WGL_ARB_create_context") != 1)
			throw new std::runtime_error("WGL_ARB_create_context is not supported");

		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, 0,
			0
		};

		m_openGLContext = wglCreateContextAttribsARB(deviceContext, 0, attributes);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempContext);
		wglMakeCurrent(deviceContext, m_openGLContext);
	}

	void DestroyOpenGLContext() {
		wglDeleteContext(m_openGLContext);
	}

private:
	HWND m_windowHandle;
	HGLRC m_openGLContext;

	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam) {
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

			windowMap[windowHandle]->CreateOpenGLContext();

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
			windowMap[windowHandle]->DestroyOpenGLContext();
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wparam, lparam);
	}
};

System::Window *System::Window::Create()
{
	return new WindowsWindow();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE parentInstance, LPSTR arguments, int commandShow)
{
	windows_instance = instance;
	windows_parentInstance = parentInstance;
	windows_arguments = arguments;
	windows_commandShow = commandShow;

	return applicationMain();
}
