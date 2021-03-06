#include <cstddef>

#include "controllerstack.hh"

ControllerStack::ControllerStack(Framework::IWindowController *windowController)
	: m_windowController(windowController)
{
}

void ControllerStack::Push(GameController *controller)
{
	if (EventHandlerStack::Size() != 0)
	{
		GameController *prev = (GameController *)EventHandlerStack::Top();
		
		prev->SetKeyboardState(NULL);
	}
	
	controller->SetKeyboardState(m_windowController->GetKeyStateReader());
	controller->SetControllerStack(this);
	
	EventHandlerStack::Push(controller);
}

void ControllerStack::Pop()
{
	if (EventHandlerStack::Size() != 0)
	{
		GameController *top = (GameController *)EventHandlerStack::Top();
		
		EventHandlerStack::Pop();
		top->SetKeyboardState(NULL);
	}

	if (EventHandlerStack::Size() != 0)
	{
		GameController *next = (GameController *)EventHandlerStack::Top();

		next->SetKeyboardState(m_windowController->GetKeyStateReader());
	}
}

void ControllerStack::Clear()
{
	if (EventHandlerStack::Size() > 0)
		Pop();
}