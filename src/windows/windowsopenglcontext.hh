#pragma once

#include <windows.h>
#include "system/openglcontext.hh"
#include "windowswindow.hh"

class WindowsOpenGLContext : public System::OpenGLContext {
public:
    WindowsOpenGLContext(HWND windowHandle);

    virtual ~WindowsOpenGLContext();

    virtual void SwapBuffers();

private:
    HGLRC m_openGLContext;
    HDC m_deviceContext;
};
