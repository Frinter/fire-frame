#pragma once

#include "framework/applicationContext.hh"

#define ApplicationThreadEntry_FunctionSignature(name) void name(Framework::IApplicationContext *applicationContext)
typedef ApplicationThreadEntry_FunctionSignature(ApplicationThreadEntry_FunctionType);
