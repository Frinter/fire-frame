#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include "framework/controllerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/ticker.hh"
#include "system/keycode.hh"
#include "system/utility.hh"

using Framework::ControllerStack;
using Framework::ReadingKeyboardState;
using Framework::ISystemTimer;
using Framework::ISleepService;
using Framework::Ticker;
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

class GraphicsThreadController : public Framework::IGraphicsThreadController
{
public:
	virtual void Run(Framework::ApplicationContext *applicationContext)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		
		while (!applicationContext->IsClosing())
		{
		}
	}
};

class OtherTestController : public Framework::GameController {
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

class TestController : public Framework::GameController {
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

class ControllerThreadController : public Framework::ILogicThreadController
{
public:
	virtual void Run(Framework::ApplicationContext *applicationContext, Framework::WindowController *windowController)
	{
		SystemTimer systemTimer;
		SleepService sleepService;
		Framework::Ticker ticker = Framework::Ticker(&systemTimer, &sleepService);
		
		unsigned int ticks = Utility::GetTicks();
		unsigned int newTicks;
	
		Framework::ControllerStack controllerStack(windowController);
		Framework::GameController *controller = new TestController();
		
		controllerStack.Push(controller);
		
		ticker.Start();
		
		while (!applicationContext->IsClosing())
		{
			controller = (Framework::GameController *)controllerStack.Top();
			controller->OnTick();

			newTicks = Utility::GetTicks();
			ticker.Wait(500);
			std::cout << "tick (" << ticks << " -> " << newTicks << ")" << std::endl;
			ticks = newTicks;			
		}
		
		controllerStack.Clear();
	}
};

Framework::IGraphicsThreadController *GetGraphicsThreadController()
{
	// memory leak
	return new GraphicsThreadController();
}

Framework::ILogicThreadController *GetLogicThreadController()
{
	// memory leak
	return new ControllerThreadController();
}
