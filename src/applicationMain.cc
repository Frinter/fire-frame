#include <iostream>

#include "system/mutex.hh"
#include "system/thread.hh"
#include "system/utility.hh"
#include "framework/applicationcontext.hh"
#include "framework/controllerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/windowcontroller.hh"

using System::Thread;
using System::ThreadEntry;
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

	ThreadEntry windowThreadEntry = [&windowController] (void*) -> void* {
		windowController.CreateWindow();
	};

	ThreadEntry controllerThreadEntry = [&applicationContext, &windowController] (void*) -> void* {
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
		
	ThreadEntry graphicsThreadEntry = [&applicationContext, &windowController] (void*) -> void* {
		applicationContext.WindowReady()->Wait();
		windowController.CreateContext();

		IGraphicsThreadController *graphicsThreadController = GetGraphicsThreadController();
		graphicsThreadController->Run();
	};
	
	Thread *windowThread = Thread::Create(windowThreadEntry);
	Thread *controllerThread = Thread::Create(controllerThreadEntry);
	Thread *graphicsThread = Thread::Create(graphicsThreadEntry);
	
	windowThread->Start();
	controllerThread->Start();
	graphicsThread->Start();
	
	graphicsThread->Wait();
	controllerThread->Wait();
	windowThread->Wait();
	
	return 0;
}
