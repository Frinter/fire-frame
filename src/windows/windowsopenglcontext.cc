#include <stdexcept>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/wglew.h>

#include "windowsopenglcontext.h"

WindowsOpenGLContext::WindowsOpenGLContext(WindowsWindow *window) {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	HDC deviceContext = GetDC(window->m_windowHandle);
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

WindowsOpenGLContext::~WindowsOpenGLContext() {
	wglDeleteContext(m_openGLContext);
}

WindowsOpenGLContext::WindowsOpenGLContext(const WindowsOpenGLContext &o) {
	throw new std::logic_error("WindowsOpenGLContext copy contructor called");
}
