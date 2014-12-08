#pragma once

#include "system/keycode.hh"

namespace Framework
{
	class IWindowController {
	public:
		IWindowController() {}

		// The implementation of Window must guarantee that Create will return before any callbacks are called
		virtual void OnWindowReady() { };
		virtual void OnWindowClose() { };
		virtual void OnKeyUp(System::KeyCode key) { };
		virtual void OnKeyDown(System::KeyCode key) { };
		
		IWindowController(const IWindowController &o) = delete;
	};
}