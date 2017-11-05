#include "framework/applicationcontext.hh"
#include "framework/clientcode.hh"

using Framework::ApplicationContext;
using Framework::ClientCode;
using Framework::LoadClientCode;

int applicationMain()
{
    ClientCode clientCode = LoadClientCode();

    ApplicationContext applicationContext;

    clientCode.ApplicationThreadEntry(&applicationContext);

    return 0;
}
