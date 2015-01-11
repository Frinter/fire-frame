#pragma once

#include "framework/applicationstate.hh"
#include "framework/applicationContext.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"

#define GetApplicationState_FunctionSignature(name) Framework::ApplicationState *name()
typedef GetApplicationState_FunctionSignature(GetApplicationState_FunctionType);

#define GraphicsThreadEntry_FunctionSignature(name) void name(Framework::IApplicationContext *applicationContext, Framework::IWindowController *windowController)
typedef GraphicsThreadEntry_FunctionSignature(GraphicsThreadEntry_FunctionType);

#define LogicThreadEntry_FunctionSignature(name) void name(Framework::IApplicationContext *applicationContext, Framework::IWindowController *windowController)
typedef LogicThreadEntry_FunctionSignature(LogicThreadEntry_FunctionType);
