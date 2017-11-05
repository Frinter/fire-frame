#include "framework/applicationcontext.hh"

#include "framework/windowcontroller.hh"

using namespace Framework;

ApplicationContext::ApplicationContext()
{
}

ApplicationContext::~ApplicationContext()
{
    for (std::vector<System::Thread*>::iterator iter = m_threads.begin(); iter != m_threads.end(); ++iter)
    {
        (*iter)->join();
    }
}

System::Window *ApplicationContext::createWindow(const char *windowName, IWindowController *windowController) {
    System::Window *window;
    auto windowThreadEntry = [&windowController, &windowName, &window] () {
        window = System::Window::Create(windowName, windowController);
        window->DoMessageLoop();
    };

    System::Thread *windowThread = new System::Thread(windowThreadEntry);
    m_threads.push_back(windowThread);
    windowController->windowReady()->Wait();

    return window;
}

System::Utility *ApplicationContext::GetSystemUtility() const
{
    return System::Utility::GetInstance();
}
