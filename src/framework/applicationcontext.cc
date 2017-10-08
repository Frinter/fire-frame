#include "framework/applicationcontext.hh"

#include "framework/windowcontroller.hh"

using namespace Framework;

ApplicationContext::ApplicationContext(ApplicationState *applicationState)
    : m_isClosing(false), m_destroyWindowFlag(false),
      m_applicationState(applicationState), m_windowReady(System::Event::Create("WindowReady")),
      m_applicationThreadQuit(System::Event::Create("GraphicsThreadQuit"))
{
}

ApplicationContext::~ApplicationContext()
{
    for (std::vector<System::thread*>::iterator iter = m_threads.begin(); iter != m_threads.end(); ++iter)
    {
        (*iter)->join();
    }
}

void ApplicationContext::Close()
{
    m_isClosing = true;
}

IWindowController *ApplicationContext::createWindow() {
    WindowController *windowController = new WindowController(this);

    ApplicationContext *self = this;
    auto windowThreadEntry = [&self, &windowController] () {
        windowController->CreateClientWindow();
        self->ApplicationThreadQuit()->Wait();
    };

    System::thread *windowThread = new System::thread(windowThreadEntry);
    m_threads.push_back(windowThread);
    WindowReady()->Wait();

    return windowController;
}

void ApplicationContext::SignalWindowDestruction()
{
    m_destroyWindowFlag = true;
}

bool ApplicationContext::IsClosing() const
{
    return m_isClosing;
}

bool ApplicationContext::ShouldDestroyWindow() const
{
    return m_isClosing || m_destroyWindowFlag;
}

System::Event *ApplicationContext::WindowReady() const
{
    return m_windowReady;
}

System::Event *ApplicationContext::ApplicationThreadQuit() const
{
    return m_applicationThreadQuit;
}

ApplicationState *ApplicationContext::GetState() const
{
    return m_applicationState;
}

System::Utility *ApplicationContext::GetSystemUtility() const
{
    return System::Utility::GetInstance();
}
