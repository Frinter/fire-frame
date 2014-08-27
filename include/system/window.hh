#pragma once

#include <stdio.h>

#include "keycode.hh"

namespace System
{
	class WindowController {
	public:
		// The implementation of Window must guarantee that Create will return before any callbacks are called
		virtual void OnWindowReady() { };
		virtual void OnKeyUp(KeyCode key) { };
		virtual void OnKeyDown(KeyCode key) { };
	};

	class Window {
	public:
		virtual ~Window() {};
		virtual int DoMessageLoop() = 0;
		static Window *Create(WindowController *controller);
	};
}
