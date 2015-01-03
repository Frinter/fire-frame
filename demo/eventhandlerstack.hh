#pragma once

#include <stack>

#include "istackeventhandler.hh"

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
