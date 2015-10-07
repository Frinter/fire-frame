#include <iostream>
#include <stdexcept>

#include "GL/gl_core_3_3.h"
#include "GL/wgl_core_3_3.h"
#include "windowsopenglcontext.hh"

WindowsOpenGLContext::WindowsOpenGLContext(WindowsWindow *window) {
    std::cout << "openGL context constructor" << std::endl;

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    m_deviceContext = GetDC(window->m_windowHandle);
    int pixelFormatIndex = ChoosePixelFormat(m_deviceContext, &pfd);
    SetPixelFormat(m_deviceContext, pixelFormatIndex, &pfd);

    HGLRC tempContext = wglCreateContext(m_deviceContext);
    wglMakeCurrent(m_deviceContext, tempContext);

    if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
        throw new std::runtime_error("Failed to initialize OpenGL functions");

    int attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    std::cout << "Creating windows openGL context" << std::endl;
    m_openGLContext = wglCreateContextAttribsARB(m_deviceContext, 0, attributes);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempContext);
    wglMakeCurrent(m_deviceContext, m_openGLContext);
}

WindowsOpenGLContext::~WindowsOpenGLContext()
{
    wglDeleteContext(m_openGLContext);
}

void WindowsOpenGLContext::SwapBuffers()
{
    ::SwapBuffers(m_deviceContext);
}
