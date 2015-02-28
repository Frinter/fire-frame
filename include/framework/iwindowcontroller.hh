#pragma once

#include "framework/iwritingkeyboardstate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

namespace Framework
{
	class ISystemWindowController {
	public:
		ISystemWindowController() {}

		// The implementation of Window must guarantee that Window::Create will return before any callbacks are called
		virtual void OnWindowReady() { };
		virtual void OnWindowClose() { };
		virtual void OnKeyUp(System::KeyCode key) { };
		virtual void OnKeyDown(System::KeyCode key) { };
		
		virtual void AddKeyboardEventHandler(IWritingKeyboardState *handler) = 0;
		virtual void RemoveKeyboardEventHandler(IWritingKeyboardState *handler) = 0;
		virtual void SwapBuffers() = 0;

		ISystemWindowController(const ISystemWindowController &o) = delete;
	};
	
	class IWindowController : public ISystemWindowController
	{
	public:
		virtual void CreateContext() = 0;
		virtual ReadingKeyboardState *GetKeyStateReader() = 0;
	};
}