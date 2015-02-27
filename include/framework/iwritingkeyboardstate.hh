#pragma once

#include "system/keycode.hh"

namespace Framework {
	class IWritingKeyboardState
	{
	public:
		virtual void PressKey(System::KeyCode key) {};
		virtual void UnpressKey(System::KeyCode key) {};
	};
}