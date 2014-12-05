#pragma once

#include <stack>

#include "framework/gamecontroller.hh"
#include "framework/windowcontroller.hh"

namespace Framework
{
	class IControllerStack
	{
	public:
		virtual void Push(GameController *controller) = 0;
		virtual void Pop() = 0;
	};
	
	class ControllerStack : public IControllerStack
	{
	public:
		ControllerStack(WindowController *windowController);
		
		virtual void Push(GameController *controller);
		virtual void Pop();
		
	private:
		WindowController *m_windowController;
		std::stack<GameController *> m_controllers;
	};
}