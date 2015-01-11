#pragma once

#include <stack>

#include "framework/iwindowcontroller.hh"

#include "icontrollerstack.hh"
#include "eventhandlerstack.hh"
#include "gamecontroller.hh"

class ControllerStack : public IControllerStack, public EventHandlerStack
{
public:
	ControllerStack(Framework::IWindowController *windowController);

	virtual void Push(GameController *controller);
	virtual void Pop();
	virtual void Clear();

private:
	Framework::IWindowController *m_windowController;
	std::stack<GameController *> m_controllers;
};
