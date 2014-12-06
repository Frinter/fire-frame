#pragma once

#include <stdio.h>

#include "system/keycode.hh"

namespace System
{
	class IWindowController {
	public:
		IWindowController() {}

		// The implementation of Window must guarantee that Create will return before any callbacks are called
		virtual void OnWindowReady() { };
		virtual void OnKeyUp(KeyCode key) { };
		virtual void OnKeyDown(KeyCode key) { };

		IWindowController(const IWindowController &o) = delete;
	};

	class Window {
	public:
		virtual ~Window() {};
		virtual int DoMessageLoop() = 0;
		static Window *Create(IWindowController *controller);
	};
}
