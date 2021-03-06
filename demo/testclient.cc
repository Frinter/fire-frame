#include <iostream>

#if TARGET_PLATFORM==MINGW
#  include "GL/gl_core_3_3.h"
#else
#  include "GL/gl.h"
#endif
#include "framework/keyboardinputhandlerinterface.hh"
#include "framework/keystate.hh"
#include "framework/platform.hh"
#include "system/keycode.hh"
#include "system/utility.hh"
#include "system/openglcontext.hh"

#include "controllerstack.hh"
#include "gamecontroller.hh"
#include "ticker.hh"

using Framework::ReadingKeyboardState;
using Framework::KeyState;
using System::KeyCode;
using System::Utility;

class SleepService : public ISleepService
{
public:
    SleepService(System::Utility *utility)
        : m_utility(utility)
    {
    }

    virtual void Sleep(unsigned int milliseconds)
    {
        m_utility->Sleep(milliseconds);
    }

private:
    System::Utility *m_utility;
};

class SystemTimer : public ISystemTimer
{
public:
    SystemTimer(System::Utility *utility)
        : m_utility(utility)
    {
    }

    virtual unsigned int GetTicks()
    {
        return m_utility->GetTicks();
    }

private:
    System::Utility *m_utility;
};

class OtherTestController : public GameController {
public:
    virtual void OnStackAdd()
    {
        std::cout << "OtherTestController added to stack" << std::endl;
    }

    virtual void OnStackRemove()
    {
        std::cout << "OtherTestController removed from stack" << std::endl;
    }

    virtual void OnStackFocus()
    {
        std::cout << "OtherTestController stack focus" << std::endl;
    }

    virtual void OnStackBlur()
    {
        std::cout << "OtherTestController stack blur" << std::endl;
    }

    virtual void OnTick()
    {
        std::cout << "OtherTestController tick" << std::endl;
    }
};

class TestController : public GameController {
public:
    TestController()
        : m_otherController(NULL)
    {
    }

    ~TestController()
    {
        if (m_otherController != NULL)
        {
            GetControllerStack()->Pop();
            delete m_otherController;
            m_otherController = NULL;
        }
    }

    virtual void OnStackAdd()
    {
        std::cout << "TestController added to stack" << std::endl;
    }

    virtual void OnStackRemove()
    {
        std::cout << "TestController removed from stack" << std::endl;
    }

    virtual void OnStackFocus()
    {
        std::cout << "TestController stack focus" << std::endl;
    }

    virtual void OnStackBlur()
    {
        std::cout << "TestController stack blur" << std::endl;
    }

    virtual void OnTick()
    {
        std::cout << "TestController tick" << std::endl;
        ReadingKeyboardState *keyboardState = GetKeyboardState();

        if (m_otherController == NULL)
        {
            m_otherController = new OtherTestController();
            GetControllerStack()->Push(m_otherController);
        }
        else
        {
            GetControllerStack()->Pop();
            delete m_otherController;
            m_otherController = NULL;
        }
    }

private:
    OtherTestController *m_otherController;
};

class KeyboardHandler : public Framework::KeyboardInputHandlerInterface
{
public:
    KeyboardHandler(System::Window *window)
        : _window(window), _fullscreen(false)
    {
    }

    virtual void PressKey(System::KeyCode key) override
    {
        if (key == System::KeyCode::KeyQ)
        {
            _window->Close();
        }

        if (key == System::KeyCode::KeyF)
        {
            if (_fullscreen)
            {
                _window->makeBordered();
                _window->setWindowPosition(100, 100, 600, 500);
                _fullscreen = false;
            }
            else
            {
                _window->makeBorderless();
                _window->makeFullscreen();
                _fullscreen = true;
            }
        }
    }

    virtual void UnpressKey(System::KeyCode key) override
    {
    }

private:
    System::Window *_window;
    bool _fullscreen;
};

void ApplicationThreadEntry(Framework::IApplicationContext *applicationContext)
{
    std::cout << "in client code" << std::endl;
    Framework::IWindowController *windowController = new Framework::WindowController();
    std::cout << "Made window controller" << std::endl;
    System::Window *window = applicationContext->createWindow("Fire Frame Demo", windowController);
    std::cout << "Made window" << std::endl;

    System::OpenGLContext *openGLContext = window->getOrCreateOpenGLContext();
    std::cout << "Made OpenGL context" << std::endl;

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "- renderer: " << renderer << std::endl;
    std::cout << "- vendor: " << vendor << std::endl;
    std::cout << "- version: " << version << std::endl;
    std::cout << "- GLSL version: " << version << std::endl;

    unsigned int framerateTicks = 1000 / 60;

    SystemTimer systemTimer(applicationContext->GetSystemUtility());
    SleepService sleepService(applicationContext->GetSystemUtility());
    Ticker ticker = Ticker(&systemTimer, &sleepService);

    unsigned int ticks = systemTimer.GetTicks();
    unsigned int newTicks;
    bool fullscreen = false;

    KeyboardHandler keyboardHandler(window);
    windowController->AddKeyboardEventHandler(&keyboardHandler);

    ControllerStack controllerStack(windowController);
    GameController *controller = new TestController();

    controllerStack.Push(controller);

    Framework::ReadingKeyboardState *keyboardState = windowController->GetKeyStateReader();

    ticker.Start();

    window->SetMousePosition(20, 20);
    while (!windowController->isWindowClosed())
    {
        controller = (GameController *)controllerStack.Top();
        controller->OnTick();

        newTicks = systemTimer.GetTicks();
        ticker.Wait(500);
        std::cout << "Application Thread (" << ticks << " -> " << newTicks << ")" << std::endl;
        ticks = newTicks;
    }

    window->Destroy();
    controllerStack.Clear();

    std::cout << "Last client message" << std::endl;
}
