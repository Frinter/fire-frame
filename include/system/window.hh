#pragma once

#include <stdio.h>

#include "framework/applicationcontext.hh"
#include "system/keycode.hh"

namespace System
{
	class IWindowController {
	public:
		IWindowController() {}

		// The implementation of Window must guarantee that Create will return before any callbacks are called
		virtual void OnWindowReady() { };
		virtual void OnWindowClose() { };
		virtual void OnKeyUp(KeyCode key) { };
		virtual void OnKeyDown(KeyCode key) { };
		
		IWindowController(const IWindowController &o) = delete;
	};

	class Window {
	public:
		virtual ~Window() {};
		virtual int DoMessageLoop() = 0;
		virtual void Destroy() = 0;
		
		static Window *Create(Framework::ApplicationContext *applicationContext, IWindowController *controller);
	};
}
