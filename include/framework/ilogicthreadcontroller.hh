#pragma once

#include "framework/applicationcontext.hh"
#include "framework/windowcontroller.hh"

namespace Framework
{
	class ILogicThreadController
	{
	public:
		virtual void Run(ApplicationContext *applicationContext, WindowController *windowController) = 0;
	};
}
