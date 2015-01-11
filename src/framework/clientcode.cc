#include <iostream>

#include "framework/clientcode.hh"

extern GetApplicationState_FunctionSignature(GetApplicationState);
extern GraphicsThreadEntry_FunctionSignature(GraphicsThreadEntry);
extern LogicThreadEntry_FunctionSignature(LogicThreadEntry);

Framework::ClientCode Framework::LoadClientCode()
{
	ClientCode clientCode;

	clientCode.GetApplicationState = GetApplicationState;
	clientCode.GraphicsThreadEntry = GraphicsThreadEntry;
	clientCode.LogicThreadEntry = LogicThreadEntry;
		
	return clientCode;
}