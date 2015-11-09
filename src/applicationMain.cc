#include "framework/applicationstate.hh"
#include "framework/applicationcontext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/windowcontroller.hh"
#include "framework/clientcode.hh"
#include "system/thread.hh"

using Framework::ApplicationState;
using Framework::ApplicationContext;
using Framework::ClientCode;
using Framework::IGraphicsThreadController;
using Framework::ILogicThreadController;
using Framework::LoadClientCode;
using Framework::WindowController;

using System::thread;

int applicationMain()
{
    ClientCode clientCode = LoadClientCode();
	
    ApplicationState *applicationState = clientCode.GetApplicationState();
    ApplicationContext applicationContext(applicationState);
    WindowController windowController(&applicationContext);
		
    auto windowThreadEntry = [&applicationContext, &windowController] () {
        windowController.CreateClientWindow();
        applicationContext.ApplicationThreadQuit()->Wait();
    };

    auto applicationThreadEntry = [&applicationContext, &windowController, &clientCode] () {
        applicationContext.WindowReady()->Wait();

        clientCode.ApplicationThreadEntry(&applicationContext, &windowController);

        applicationContext.SignalWindowDestruction();
        applicationContext.ApplicationThreadQuit()->Trigger();
    };

    thread windowThread = thread(windowThreadEntry);
    thread applicationThread = thread(applicationThreadEntry);

    applicationThread.join();
    windowThread.join();

    return 0;
}
