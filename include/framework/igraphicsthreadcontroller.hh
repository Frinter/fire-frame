#pragma once

#include "framework/applicationcontext.hh"

namespace Framework
{
	class IGraphicsThreadController
	{
	public:
		virtual void Run(ApplicationContext *applicationContext) = 0;
	};
}