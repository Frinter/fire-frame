#include <iostream>

#if PLATFORM == windows
  #define GLEW_STATIC 1
  #include <GL/glew.h>
#endif

#include "framework/applicationstate.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/readingkeyboardstate.hh"
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
	virtual void Sleep(unsigned int milliseconds)
	{
		Utility::Sleep(milliseconds);
	}
};

class SystemTimer : public ISystemTimer
{
public:
	virtual unsigned int GetTicks()
	{
		return Utility::GetTicks();
	}
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

		std::cout << "TestController: ";
		if (keyboardState != NULL) {
			std::cout << keyboardState->GetKeyState(KeyCode::KeyTab);
		}
		else {
			std::cout << "No state";
		}
		std::cout << std::endl;

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

class LogicThreadController : public Framework::ILogicThreadController
{
public:
	virtual void Run(Framework::ApplicationContext *applicationContext, Framework::WindowController *windowController)
	{
		SystemTimer systemTimer;
		SleepService sleepService;
		Ticker ticker = Ticker(&systemTimer, &sleepService);
		
		unsigned int ticks = Utility::GetTicks();
		unsigned int newTicks;
	
		ControllerStack controllerStack(windowController);
		GameController *controller = new TestController();
		
		controllerStack.Push(controller);
		
		ticker.Start();
		
		while (!applicationContext->IsClosing())
		{
			controller = (GameController *)controllerStack.Top();
			controller->OnTick();

			newTicks = Utility::GetTicks();
			ticker.Wait(500);
			std::cout << "Logic Thread (" << ticks << " -> " << newTicks << ")" << std::endl;
			ticks = newTicks;			
		}
		
		controllerStack.Clear();
	}
};

class GraphicsThreadController : public Framework::IGraphicsThreadController
{
public:
	virtual void Run(Framework::ApplicationContext *applicationContext)
	{
		SystemTimer systemTimer;
		SleepService sleepService;
		Ticker ticker = Ticker(&systemTimer, &sleepService);

		unsigned int ticks = Utility::GetTicks();
		unsigned int newTicks;

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		ticker.Start();

		while (!applicationContext->IsClosing())
		{
			newTicks = Utility::GetTicks();
			ticker.Wait(100);
			std::cout << "Graphics Thread (" << ticks << " -> " << newTicks << ")" << std::endl;
			ticks = newTicks;			
		}
	}
};

Framework::ApplicationState applicationState = {
	.windowName = "Fire Frame Demo"
};

Framework::ApplicationState *GetApplicationState()
{
	return &applicationState;
}

Framework::IGraphicsThreadController *GetGraphicsThreadController()
{
	// memory leak
	return new GraphicsThreadController();
}

Framework::ILogicThreadController *GetLogicThreadController()
{
	// memory leak
	return new LogicThreadController();
}
