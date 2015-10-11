#include <iostream>

#include "GL/gl_core_3_3.h"
#include "framework/platform.hh"
#include "system/keycode.hh"
#include "system/utility.hh"

#include "controllerstack.hh"
#include "gamecontroller.hh"
#include "ticker.hh"

using Framework::ReadingKeyboardState;
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

static Framework::ApplicationState applicationState = {
    .windowName = "Fire Frame Demo"
};

GetApplicationState_FunctionSignature(GetApplicationState)
{
    return &applicationState;
}

GraphicsThreadEntry_FunctionSignature(GraphicsThreadEntry)
{
    std::cout << "GraphicsThreadEntry: " << std::endl;

    windowController->CreateContext();

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
    std::cout << "- renderer: " << renderer << std::endl;
    std::cout << "- vendor: " << vendor << std::endl;
    std::cout << "- version: " << version << std::endl;
    std::cout << "- GLSL version: " << version << std::endl;

    //GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    unsigned int framerateTicks = 1000 / 60;

    SystemTimer systemTimer(applicationContext->GetSystemUtility());
    SleepService sleepService(applicationContext->GetSystemUtility());
    Ticker ticker = Ticker(&systemTimer, &sleepService);
    ticker.Start();

    while (!applicationContext->IsClosing())
    {
        ticker.Wait(framerateTicks);
    }
}

LogicThreadEntry_FunctionSignature(LogicThreadEntry)
{
    SystemTimer systemTimer(applicationContext->GetSystemUtility());
    SleepService sleepService(applicationContext->GetSystemUtility());
    Ticker ticker = Ticker(&systemTimer, &sleepService);

    unsigned int ticks = systemTimer.GetTicks();
    unsigned int newTicks;

    ControllerStack controllerStack(windowController);
    GameController *controller = new TestController();

    controllerStack.Push(controller);

    ticker.Start();

    while (!applicationContext->IsClosing())
    {
        controller = (GameController *)controllerStack.Top();
        controller->OnTick();
        
        newTicks = systemTimer.GetTicks();
        ticker.Wait(500);
        std::cout << "Logic Thread (" << ticks << " -> " << newTicks << ")" << std::endl;
        ticks = newTicks;			
    }
    
    controllerStack.Clear();
}
