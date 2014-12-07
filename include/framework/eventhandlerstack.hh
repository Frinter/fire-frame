#pragma once

#include <stack>

#include "framework/istackeventhandler.hh"

namespace Framework
{
	class EventHandlerStack
	{
	public:
		virtual void Push(IStackEventHandler *handler);
		virtual void Pop();
		
		virtual IStackEventHandler *Top();
		virtual int Size();
		
	private:
		std::stack<IStackEventHandler *> m_handlers;
	};
}