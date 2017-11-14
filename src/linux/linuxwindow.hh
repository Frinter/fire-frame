#include "system/window.hh"
#include "X11/Xlib.h"

class LinuxWindow : public System::Window
{
public:
    LinuxWindow();
    virtual ~LinuxWindow();

    virtual int DoMessageLoop();
    virtual void GetWindowSize(unsigned int *width, unsigned int *height);
    virtual bool SetMousePosition(unsigned int posX, unsigned int posY);
    virtual void Destroy();
    virtual void Close();

private:
    Display *_display;
    ::Window _window;
};
