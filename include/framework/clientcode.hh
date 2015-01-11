#pragma once

#include "framework/applicationstate.hh"
#include "framework/clientdefinitions.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/ilogicthreadcontroller.hh"

namespace Framework
{
	struct ClientCode
	{
		GetApplicationState_FunctionType *GetApplicationState;
		GraphicsThreadEntry_FunctionType *GraphicsThreadEntry;
		LogicThreadEntry_FunctionType *LogicThreadEntry;
	};
	
	ClientCode LoadClientCode();
}