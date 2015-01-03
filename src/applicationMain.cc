#include <iostream>
#include <thread>

#include "framework/applicationstate.hh"
#include "framework/applicationcontext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/windowcontroller.hh"

using Framework::ApplicationState;
using Framework::ApplicationContext;
using Framework::IGraphicsThreadController;
using Framework::ILogicThreadController;
using Framework::WindowController;

extern ApplicationState *GetApplicationState();
extern IGraphicsThreadController *GetGraphicsThreadController();
extern ILogicThreadController *GetLogicThreadController();

int applicationMain()
{
	ApplicationState *applicationState = GetApplicationState();
	ApplicationContext applicationContext(applicationState);
	WindowController windowController(&applicationContext);
		
	auto windowThreadEntry = [&applicationContext, &windowController] () {
		windowController.CreateWindow();
		applicationContext.GraphicsThreadQuit()->Wait();
	};

	auto logicThreadEntry = [&applicationContext, &windowController] () {
		ILogicThreadController *logicThreadController = GetLogicThreadController();
		logicThreadController->Run(&applicationContext, &windowController);

		applicationContext.SignalWindowDestruction();
	};

	auto graphicsThreadEntry = [&applicationContext, &windowController] () {
		applicationContext.WindowReady()->Wait();
		windowController.CreateContext();

		IGraphicsThreadController *graphicsThreadController = GetGraphicsThreadController();
		graphicsThreadController->Run(&applicationContext);

		applicationContext.GraphicsThreadQuit()->Trigger();
	};

	std::thread windowThread = std::thread(windowThreadEntry);
	std::thread logicThread = std::thread(logicThreadEntry);
	std::thread graphicsThread = std::thread(graphicsThreadEntry);
	
	graphicsThread.join();
	logicThread.join();
	windowThread.join();
	
	return 0;
}
