#pragma once

#include "framework/clientdefinitions.hh"

namespace Framework
{
    struct ClientCode
    {
        ApplicationThreadEntry_FunctionType *ApplicationThreadEntry;
    };

    ClientCode LoadClientCode();
}
