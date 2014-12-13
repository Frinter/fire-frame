#include <thread>

#include "system/mutex.hh"
#include "system/utility.hh"
#include "framework/applicationcontext.hh"
#include "framework/controllerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/windowcontroller.hh"

using System::Utility;
using System::KeyCode;
using Framework::ApplicationContext;
using Framework::ControllerStack;
using Framework::GameController;
using Framework::IGraphicsThreadController;
using Framework::WindowController;

extern IGraphicsThreadController *GetGraphicsThreadController();
extern GameController *GetBaseController();

unsigned int GetSleepLength(unsigned int newTicks, unsigned int originalTicks, unsigned int length)
{
	unsigned int difference = newTicks - originalTicks;
	return difference > length ? 0 : length - difference;
}

int applicationMain()
{
	ApplicationContext applicationContext;
	WindowController windowController(&applicationContext);
		
	auto windowThreadEntry = [&applicationContext, &windowController] () {
		windowController.CreateWindow();
		applicationContext.GraphicsThreadQuit()->Wait();
	};

	auto controllerThreadEntry = [&applicationContext, &windowController] () {
		ControllerStack controllerStack(&windowController);
		GameController *controller = GetBaseController();
		
		controllerStack.Push(controller);
		
		unsigned int ticks = Utility::GetTicks();
		unsigned int newTicks;
		unsigned int sleepLength;

		while (!applicationContext.IsClosing())
		{
			controller = (GameController *)controllerStack.Top();
			controller->OnTick();

			newTicks = Utility::GetTicks();
			sleepLength = GetSleepLength(newTicks, ticks, 500);
			ticks = newTicks;

			if (sleepLength > 0)
				Utility::Sleep(sleepLength);
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
