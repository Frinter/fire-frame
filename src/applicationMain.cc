#include <thread>

#include "framework/applicationstate.hh"
#include "framework/applicationcontext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/windowcontroller.hh"
#include "framework/clientcode.hh"

using Framework::ApplicationState;
using Framework::ApplicationContext;
using Framework::ClientCode;
using Framework::IGraphicsThreadController;
using Framework::ILogicThreadController;
using Framework::LoadClientCode;
using Framework::WindowController;

int applicationMain()
{
	ClientCode clientCode = LoadClientCode();
	
	ApplicationState *applicationState = clientCode.GetApplicationState();
	ApplicationContext applicationContext(applicationState);
	WindowController windowController(&applicationContext);
		
	auto windowThreadEntry = [&applicationContext, &windowController] () {
		windowController.CreateWindow();
		applicationContext.GraphicsThreadQuit()->Wait();
	};

	auto logicThreadEntry = [&applicationContext, &windowController, &clientCode] () {
		clientCode.LogicThreadEntry(&applicationContext, &windowController);

		applicationContext.SignalWindowDestruction();
	};

	auto graphicsThreadEntry = [&applicationContext, &windowController, &clientCode] () {
		applicationContext.WindowReady()->Wait();

		clientCode.GraphicsThreadEntry(&applicationContext, &windowController);

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
