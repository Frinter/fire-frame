#include <iostream>
#include <thread>

#include "system/mutex.hh"
#include "system/utility.hh"
#include "framework/applicationcontext.hh"
#include "framework/controllerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/windowcontroller.hh"
#include "framework/ticker.hh"

using System::Utility;
using System::KeyCode;
using Framework::ApplicationContext;
using Framework::ControllerStack;
using Framework::GameController;
using Framework::IGraphicsThreadController;
using Framework::ISleepService;
using Framework::ISystemTimer;
using Framework::Ticker;
using Framework::WindowController;

extern IGraphicsThreadController *GetGraphicsThreadController();
extern GameController *GetBaseController();

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

int applicationMain()
{
	ApplicationContext applicationContext;
	WindowController windowController(&applicationContext);
		
	auto windowThreadEntry = [&applicationContext, &windowController] () {
		windowController.CreateWindow();
		applicationContext.GraphicsThreadQuit()->Wait();
	};

	auto controllerThreadEntry = [&applicationContext, &windowController] () {
		SystemTimer systemTimer;
		SleepService sleepService;
		Ticker ticker = Ticker(&systemTimer, &sleepService);
		
		unsigned int ticks = Utility::GetTicks();
		unsigned int newTicks;
	
		ControllerStack controllerStack(&windowController);
		GameController *controller = GetBaseController();
		
		controllerStack.Push(controller);
		
		ticker.Start();
		
		while (!applicationContext.IsClosing())
		{
			controller = (GameController *)controllerStack.Top();
			controller->OnTick();

			newTicks = Utility::GetTicks();
			ticker.Wait(700);
			std::cout << "tick (" << ticks << " -> " << newTicks << ")" << std::endl;
			ticks = newTicks;			
		}
		
		controllerStack.Clear();

		applicationContext.SignalWindowDestruction();
	};

	auto graphicsThreadEntry = [&applicationContext, &windowController] () {
		applicationContext.WindowReady()->Wait();
		windowController.CreateContext();

		IGraphicsThreadController *graphicsThreadController = GetGraphicsThreadController();
		graphicsThreadController->Run(&applicationContext);

		applicationContext.GraphicsThreadQuit()->Trigger();;
	};

	std::thread windowThread = std::thread(windowThreadEntry);
	std::thread controllerThread = std::thread(controllerThreadEntry);
	std::thread graphicsThread = std::thread(graphicsThreadEntry);
	
	graphicsThread.join();
	controllerThread.join();
	windowThread.join();
	
	return 0;
}
