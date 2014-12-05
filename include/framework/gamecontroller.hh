#pragma once

#include "framework/icontrollerstack.hh"
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

namespace Framework
{
	class GameController {
	public:
		GameController();
		virtual ~GameController() {}
		
		void SetKeyboardState(ReadingKeyboardState *state);
		void SetControllerStack(IControllerStack *stack);
		
		// void *OnInit();
		// void *OnDestroy();
		
	protected:
		ReadingKeyboardState *GetKeyboardState();
		IControllerStack *GetControllerStack();
		
	private:
		IControllerStack *m_controllerStack;
		ReadingKeyboardState *m_keyboardState;
	};
}