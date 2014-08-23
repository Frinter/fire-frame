#pragma once

#include <windows.h>

#include "../window.h"

class WindowsOpenGLContext;

class WindowsWindow : public System::Window
{
	friend class WindowsOpenGLContext;

public:
	WindowsWindow(HINSTANCE processInstance, int commandShow);

	virtual ~WindowsWindow();

	virtual int DoMessageLoop();

private:
	HWND m_windowHandle;
	WindowsOpenGLContext *m_openGLContext;

	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};
