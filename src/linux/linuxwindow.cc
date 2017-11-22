#include "linuxwindow.hh"

LinuxWindow::LinuxWindow(Framework::ISystemWindowController *controller)
    : _controller(controller)
{
    _display = XOpenDisplay(NULL);
    _window = XCreateSimpleWindow(
        _display, DefaultRootWindow(_display),
        0, 0, 100, 200, 0,
        BlackPixel(_display, DefaultScreen(_display)),
        WhitePixel(_display, DefaultScreen(_display))
    );

    XSelectInput(_display, _window, StructureNotifyMask);

    XMapWindow(_display, _window);
}

LinuxWindow::~LinuxWindow()
{
}

int LinuxWindow::DoMessageLoop()
{
    _controller->OnWindowReady();

    bool running = true;
    while (running) {
        XEvent event;
        XNextEvent(_display, &event);

        if (event.type == MapNotify)
        {
            running = false;
            break;
        }
    }

    return 0;
}

void LinuxWindow::GetWindowSize(unsigned int *width, unsigned int *height)
{
}

bool LinuxWindow::SetMousePosition(unsigned int posX, unsigned int posY)
{
    return false;
}

void LinuxWindow::Destroy()
{
}

void LinuxWindow::Close()
{
}

::Window LinuxWindow::getWindowHandle() const
{
    return _window;
}

Display *LinuxWindow::getDisplay() const
{
    return _display;
}

System::Window *System::Window::Create(const char *windowName, Framework::ISystemWindowController *controller)
{
    return new LinuxWindow(controller);
}
