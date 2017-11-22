#include <GL/glx.h>

#include "linuxwindow.hh"
#include "system/openglcontext.hh"

class LinuxOpenGLContext : public System::OpenGLContext
{
public:
    LinuxOpenGLContext(System::Window *window)
        : _window((LinuxWindow *)window)
    {
        Display *display = _window->getDisplay();
        ::Window windowHandle = _window->getWindowHandle();
        GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

        XVisualInfo *visualInfo = glXChooseVisual(display, 0, attributes);
        Colormap colorMap = XCreateColormap(display, windowHandle, visualInfo->visual, AllocNone);

        _context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);

        glXMakeCurrent(display, windowHandle, _context);
    }

    ~LinuxOpenGLContext()
    {
    }

    void SwapBuffers()
    {
    }

private:
    LinuxWindow *_window;
    GLXContext _context;
};

System::OpenGLContext *System::OpenGLContext::Create(System::Window *window)
{
    return new LinuxOpenGLContext(window);
}
