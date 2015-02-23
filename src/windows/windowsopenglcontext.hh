#pragma once

#include <windows.h>
#include "system/openglcontext.hh"
#include "windowswindow.hh"

class WindowsOpenGLContext : public System::OpenGLContext {
public:
	WindowsOpenGLContext(WindowsWindow *window);

	virtual ~WindowsOpenGLContext();

	virtual void SwapBuffers();
	
private:
	HGLRC m_openGLContext;
	HDC m_deviceContext;

	WindowsOpenGLContext(const WindowsOpenGLContext &o) = delete;
};
