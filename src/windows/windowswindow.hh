#pragma once

#include <windows.h>

#include "system/window.hh"

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
	void KeyDown(WPARAM key);
	void KeyUp(WPARAM key);

	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};
