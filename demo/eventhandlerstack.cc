#include "eventhandlerstack.hh"

void EventHandlerStack::Push(IStackEventHandler *handler)
{
	if (m_handlers.size() != 0)
	{
		IStackEventHandler *prev = m_handlers.top();
		
		prev->OnStackBlur();
	}
	
	m_handlers.push(handler);

	handler->OnStackAdd();
	handler->OnStackFocus();
}

void EventHandlerStack::Pop()
{
	if (m_handlers.size() != 0)
	{
		IStackEventHandler *top = m_handlers.top();
		
		top->OnStackBlur();
		top->OnStackRemove();
		
		m_handlers.pop();
	}

	if (m_handlers.size() != 0)
	{
		IStackEventHandler *next = m_handlers.top();

		next->OnStackFocus();
	}
}

IStackEventHandler *EventHandlerStack::Top()
{
	return m_handlers.top();
}

int EventHandlerStack::Size()
{
	return m_handlers.size();
}
