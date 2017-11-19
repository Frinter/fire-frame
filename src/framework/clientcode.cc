#include "framework/clientcode.hh"

extern ApplicationThreadEntry_FunctionSignature(ApplicationThreadEntry);

Framework::ClientCode Framework::LoadClientCode()
{
    ClientCode clientCode;

    clientCode.ApplicationThreadEntry = ApplicationThreadEntry;

    return clientCode;
}
