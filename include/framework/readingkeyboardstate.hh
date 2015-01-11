#pragma once

#include "framework/keystate.hh"
#include "system/keycode.hh"

namespace Framework {
	class ReadingKeyboardState
	{
	public:
		virtual KeyState GetKeyState(System::KeyCode key) = 0;
	};
}