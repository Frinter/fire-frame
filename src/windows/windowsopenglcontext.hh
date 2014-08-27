#pragma once

#include <windows.h>
#include "system/openglcontext.hh"
#include "windowswindow.hh"

class WindowsOpenGLContext : public System::OpenGLContext {
public:
	WindowsOpenGLContext(WindowsWindow *window);

	virtual ~WindowsOpenGLContext();

private:
	HGLRC m_openGLContext;

	WindowsOpenGLContext(const WindowsOpenGLContext &o);
};
