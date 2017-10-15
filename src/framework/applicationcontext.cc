#include "framework/applicationcontext.hh"

#include "framework/windowcontroller.hh"

using namespace Framework;

ApplicationContext::ApplicationContext()
{
}

ApplicationContext::~ApplicationContext()
{
    for (std::vector<System::thread*>::iterator iter = m_threads.begin(); iter != m_threads.end(); ++iter)
    {
        (*iter)->join();
    }
}

IWindowController *ApplicationContext::createWindow(const char *windowName) {
    WindowController *windowController = new WindowController(this);

    ApplicationContext *self = this;
    auto windowThreadEntry = [&self, &windowController, &windowName] () {
        System::Window *window = System::Window::Create(self, windowName, windowController);
        windowController->setWindow(window);
    };

    System::thread *windowThread = new System::thread(windowThreadEntry);
    m_threads.push_back(windowThread);
    windowController->windowReady()->Wait();

    return windowController;
}

System::Utility *ApplicationContext::GetSystemUtility() const
{
    return System::Utility::GetInstance();
}
