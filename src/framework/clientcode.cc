#include <iostream>

#include "framework/clientcode.hh"

extern GetApplicationState_FunctionSignature(GetApplicationState);
extern ApplicationThreadEntry_FunctionSignature(ApplicationThreadEntry);

Framework::ClientCode Framework::LoadClientCode()
{
    ClientCode clientCode;

    clientCode.GetApplicationState = GetApplicationState;
    clientCode.ApplicationThreadEntry = ApplicationThreadEntry;
		
    return clientCode;
}
