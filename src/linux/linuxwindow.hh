#include <X11/Xlib.h>
#include "system/window.hh"

class LinuxWindow : public System::Window
{
public:
    LinuxWindow(Framework::ISystemWindowController *controller);
    virtual ~LinuxWindow();

    virtual int DoMessageLoop();
    virtual void GetWindowSize(unsigned int *width, unsigned int *height);
    virtual bool SetMousePosition(unsigned int posX, unsigned int posY);
    virtual void Destroy();
    virtual void Close();

    Display *getDisplay() const;
    ::Window getWindowHandle() const;

private:
    Display *_display;
    ::Window _window;
    Framework::ISystemWindowController *_controller;
};
