#pragma once

#include <map>

#include "keycode.hh"
#include "window.hh"
#include "openglcontext.hh"
#include "mutex.hh"

namespace System
{
	enum KeyState
	{
		Unpressed,
		Pressed
	};

	class SystemWindowController : public WindowController
	{
	public:
		class ReadingKeyboardState
		{
		public:
			virtual KeyState GetKeyState(KeyCode key) = 0;
		};

	private:
		class WritingKeyboardState
		{
		public:
			virtual void PressKey(KeyCode key) = 0;
			virtual void UnpressKey(KeyCode key) = 0;
		};

		class KeyboardState : public ReadingKeyboardState, public WritingKeyboardState
		{
		public:
			KeyboardState();
			virtual ~KeyboardState();

			virtual KeyState GetKeyState(KeyCode key);
			virtual void PressKey(KeyCode key);
			virtual void UnpressKey(KeyCode key);

		private:
			Mutex *m_mutex;
			std::map<KeyCode, KeyState> m_states;
		};

	public:
		SystemWindowController();

		virtual ~SystemWindowController();

		void CreateWindow();

		ReadingKeyboardState *GetKeyStateReader();

		virtual void OnWindowReady();
		virtual void OnKeyDown(KeyCode key);
		virtual void OnKeyUp(KeyCode key);
		
	private:
		Window *m_window;
		OpenGLContext *m_openGLContext;
		KeyboardState m_keyboardState;
	};
}
