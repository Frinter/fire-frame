#pragma once

#include "system/keycode.hh"

namespace Framework {
	class ReadingKeyboardState
	{
	public:
		virtual KeyState GetKeyState(System::KeyCode key) = 0;
	};
}