#pragma once

#include "framework/istackeventhandler.hh"
#include "framework/icontrollerstack.hh"
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

namespace Framework
{
	class GameController : public Framework::IStackEventHandler {
	public:
		GameController();
		virtual ~GameController() {}
		
		void SetKeyboardState(ReadingKeyboardState *state);
		void SetControllerStack(IControllerStack *stack);
		
		virtual void OnStackAdd() {};
		virtual void OnStackRemove() {};
		virtual void OnStackFocus() {};
		virtual void OnStackBlur() {};
		
	protected:
		ReadingKeyboardState *GetKeyboardState();
		IControllerStack *GetControllerStack();
		
	private:
		IControllerStack *m_controllerStack;
		ReadingKeyboardState *m_keyboardState;
	};
}