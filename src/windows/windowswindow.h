#pragma once

#include <windows.h>

#include "../window.h"

class WindowsOpenGLContext;

class WindowsWindow : public System::Window
{
	friend class WindowsOpenGLContext;

public:
	WindowsWindow(System::WindowController *controller, HINSTANCE processInstance, int commandShow);
	virtual ~WindowsWindow();

	virtual int DoMessageLoop();

private:
	HWND m_windowHandle;
	System::WindowController *m_controller;
	WindowsOpenGLContext *m_openGLContext;

	void Ready();

	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};
