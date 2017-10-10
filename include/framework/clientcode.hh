#pragma once

#include "framework/clientdefinitions.hh"
#include "framework/ilogicthreadcontroller.hh"

namespace Framework
{
    struct ClientCode
    {
        ApplicationThreadEntry_FunctionType *ApplicationThreadEntry;
    };

    ClientCode LoadClientCode();
}
