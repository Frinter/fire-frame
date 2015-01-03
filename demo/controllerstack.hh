#pragma once

#include <stack>

#include "framework/windowcontroller.hh"

#include "icontrollerstack.hh"
#include "eventhandlerstack.hh"
#include "gamecontroller.hh"

class ControllerStack : public IControllerStack, public EventHandlerStack
{
public:
	ControllerStack(Framework::WindowController *windowController);

	virtual void Push(GameController *controller);
	virtual void Pop();
	virtual void Clear();

private:
	Framework::WindowController *m_windowController;
	std::stack<GameController *> m_controllers;
};
