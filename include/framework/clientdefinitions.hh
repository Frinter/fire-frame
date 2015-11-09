#pragma once

#include "framework/applicationstate.hh"
#include "framework/applicationContext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"

#define GetApplicationState_FunctionSignature(name) Framework::ApplicationState *name()
typedef GetApplicationState_FunctionSignature(GetApplicationState_FunctionType);

#define ApplicationThreadEntry_FunctionSignature(name) void name(Framework::IApplicationContext *applicationContext, Framework::IWindowController *windowController)
typedef ApplicationThreadEntry_FunctionSignature(ApplicationThreadEntry_FunctionType);
