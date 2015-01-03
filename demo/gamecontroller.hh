#pragma once

#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

#include "istackeventhandler.hh"
#include "icontrollerstack.hh"

class GameController : public IStackEventHandler {
public:
	// needs a way to pop itself off the stack
	GameController();
	virtual ~GameController() {}

	void SetControllerStack(IControllerStack *stack);
	void SetKeyboardState(Framework::ReadingKeyboardState *state);

	virtual void OnStackAdd() {}
	virtual void OnStackRemove() {}
	virtual void OnStackFocus() {}
	virtual void OnStackBlur() {}

	virtual void OnTick() {}

protected:
	IControllerStack *GetControllerStack();
	Framework::ReadingKeyboardState *GetKeyboardState();

private:
	IControllerStack *m_controllerStack;
	Framework::ReadingKeyboardState *m_keyboardState;
};
