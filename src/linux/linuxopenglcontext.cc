#include <GL/glx.h>

#include "system/openglcontext.hh"
#include "linuxwindow.hh"

class LinuxOpenGLContext : public System::OpenGLContext
{
public:
    LinuxOpenGLContext(LinuxWindow *window)
        : _window(window)
    {
        ::Display *display = _window->getDisplay();
        ::Window windowHandle = _window->getWindowHandle();
        GLint attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

        XVisualInfo *visualInfo = glXChooseVisual(display, 0, attributes);
        Colormap colorMap = XCreateColormap(display, windowHandle, visualInfo->visual, AllocNone);

        _context = glXCreateContext(display, visualInfo, NULL, GL_TRUE);

        glXMakeCurrent(display, windowHandle, _context);
    }

    ~LinuxOpenGLContext()
    {
        glXDestroyContext(_window->getDisplay(), _context);
    }

    void SwapBuffers()
    {
        glXSwapBuffers(_window->getDisplay(), _window->getWindowHandle());
    }

private:
    LinuxWindow *_window;
    GLXContext _context;
};

System::OpenGLContext *System::OpenGLContext::Create(System::Window *window)
{
    return new LinuxOpenGLContext((LinuxWindow *)window);
}
