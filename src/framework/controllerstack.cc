#include <cstddef>

#include "framework/controllerstack.hh"

Framework::ControllerStack::ControllerStack(WindowController *windowController)
	: m_windowController(windowController)
{
}

void Framework::ControllerStack::Push(GameController *controller)
{
	if (m_controllers.size() != 0)
	{
		GameController *prev = m_controllers.top();
		prev->SetKeyboardState(NULL);
	}
	
	controller->SetKeyboardState(m_windowController->GetKeyStateReader());
	controller->SetControllerStack(this);
	m_controllers.push(controller);
}

void Framework::ControllerStack::Pop()
{
	if (m_controllers.size() != 0)
	{
		GameController *top = m_controllers.top();
		m_controllers.pop();
		top->SetKeyboardState(NULL);
	}

	if (m_controllers.size() != 0)
	{
		GameController *next = m_controllers.top();

		next->SetKeyboardState(m_windowController->GetKeyStateReader());
	}
}
