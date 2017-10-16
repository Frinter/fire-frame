#include <windows.h>

#include "windowswindow.hh"
#include "windowsopenglcontext.hh"

extern int applicationMain();

class WindowsWindow;

HINSTANCE windows_instance;
HINSTANCE windows_parentInstance;
LPSTR windows_arguments;
int windows_commandShow;

System::Window *System::Window::Create(const char *windowName, Framework::ISystemWindowController *controller)
{
    return new WindowsWindow(windowName, controller, windows_instance, windows_commandShow);
}

System::OpenGLContext *System::OpenGLContext::Create(Window *window)
{
    return new WindowsOpenGLContext(dynamic_cast<WindowsWindow*>(window));
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE parentInstance, LPSTR arguments, int commandShow)
{
    windows_instance = instance;
    windows_parentInstance = parentInstance;
    windows_arguments = arguments;
    windows_commandShow = commandShow;

    return applicationMain();
}
