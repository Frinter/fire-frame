#pragma once

#include <stdio.h>

namespace System
{
	enum KeyCode {
		KeyUnknown,
		KeyEscape,
		KeyBackspace,
		KeyTab,
		KeyEnter,
		KeyShift,
		KeyControl,
		KeyAlt,
		KeyCapsLock,
		KeySpace,
		KeyPageUp,
		KeyPageDown,
		KeyEnd,
		KeyHome,
		KeyInsert,
		KeyDelete,

		KeyRightControl, KeyLeftControl,
		KeyRightShift, KeyLeftShift,

		KeyLeftArrow, KeyRightArrow, KeyUpArrow, KeyDownArrow,

		Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,

		KeyNumpad0, KeyNumpad1, KeyNumpad2, KeyNumpad3, KeyNumpad4, KeyNumpad5, KeyNumpad6, KeyNumpad7, KeyNumpad8, KeyNumpad9,
		KeyMultiply, KeyPlus, KeyMinus, KeyDivide, KeyDecimal,

 		KeyA,
 		KeyB,
 		KeyC,
 		KeyD,
 		KeyE,
 		KeyF,
 		KeyG,
 		KeyH,
 		KeyI,
 		KeyJ,
 		KeyK,
 		KeyL,
 		KeyM,
 		KeyN,
 		KeyO,
 		KeyP,
 		KeyQ,
 		KeyR,
 		KeyS,
 		KeyT,
 		KeyU,
 		KeyV,
		KeyW,
 		KeyX,
 		KeyY,
 		KeyZ,
	};

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

	class OpenGLContext {
	public:
		virtual ~OpenGLContext() {};

		static OpenGLContext *Create(Window *window);
	};
}
