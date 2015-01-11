#pragma once

#include <windows.h>

#include "system/window.hh"

class WindowsOpenGLContext;

class WindowsWindow : public System::Window
{
	friend class WindowsOpenGLContext;

public:
	WindowsWindow(Framework::ApplicationContext *applicationContext, Framework::ISystemWindowController *controller, HINSTANCE processInstance, int commandShow);
	virtual ~WindowsWindow();

	virtual int DoMessageLoop();
	
	virtual void Destroy();

private:
	HWND m_windowHandle;
	Framework::ISystemWindowController *m_controller;
	Framework::ApplicationContext *m_applicationContext;
	WindowsOpenGLContext *m_openGLContext;

	void Ready();
	void Close();
	void KeyDown(WPARAM key);
	void KeyUp(WPARAM key);

	WindowsWindow(const WindowsWindow &o) = delete;

	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};
