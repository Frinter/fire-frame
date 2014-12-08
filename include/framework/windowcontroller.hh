#pragma once

#include <map>

#include "framework/applicationcontext.hh"
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "framework/iwindowcontroller.hh"
#include "system/keycode.hh"
#include "system/window.hh"
#include "system/openglcontext.hh"
#include "system/mutex.hh"

namespace Framework
{
	class WindowController : public IWindowController
	{
	private:
		class WritingKeyboardState
		{
		public:
			virtual void PressKey(System::KeyCode key) = 0;
			virtual void UnpressKey(System::KeyCode key) = 0;
		};

		class KeyboardState : public ReadingKeyboardState, public WritingKeyboardState
		{
		public:
			KeyboardState();
			virtual ~KeyboardState();

			virtual KeyState GetKeyState(System::KeyCode key);
			virtual void PressKey(System::KeyCode key);
			virtual void UnpressKey(System::KeyCode key);

		private:
			System::Mutex *m_mutex;
			std::map<System::KeyCode, KeyState> m_states;

			KeyboardState(const KeyboardState &o) = delete;
			KeyboardState(KeyboardState &&o) = delete;
		};

	public:
		WindowController(ApplicationContext *applicationContext);

		virtual ~WindowController();

		void CreateWindow();

		ReadingKeyboardState *GetKeyStateReader();

		virtual void OnWindowReady();
		virtual void OnWindowClose();
		virtual void OnKeyDown(System::KeyCode key);
		virtual void OnKeyUp(System::KeyCode key);
		
	private:
		System::Window *m_window;
		System::OpenGLContext *m_openGLContext;
		ApplicationContext *m_applicationContext;
		KeyboardState m_keyboardState;
		
		bool m_shouldDestroyWindow;
	};
}
