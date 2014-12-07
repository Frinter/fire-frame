#pragma once

#include <stack>

#include "framework/icontrollerstack.hh"
#include "framework/eventhandlerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/windowcontroller.hh"

namespace Framework
{
	class ControllerStack : public IControllerStack, public EventHandlerStack
	{
	public:
		ControllerStack(WindowController *windowController);
		
		virtual void Push(GameController *controller);
		virtual void Pop();
		virtual void Clear();
		
	private:
		WindowController *m_windowController;
		std::stack<GameController *> m_controllers;
	};
}