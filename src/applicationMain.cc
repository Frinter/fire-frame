#include "framework/applicationcontext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"
#include "framework/windowcontroller.hh"
#include "framework/clientcode.hh"
#include "system/thread.hh"

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

    ApplicationContext applicationContext;

    clientCode.ApplicationThreadEntry(&applicationContext);

    applicationContext.SignalWindowDestruction();
    applicationContext.ApplicationThreadQuit()->Trigger();

    return 0;
}
