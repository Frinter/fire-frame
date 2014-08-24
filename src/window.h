#pragma once

#include <stdio.h>

namespace System
{
	class WindowController {
	public:
		// The implementation of Window must guarantee that Create will return before OnWindowReady is called
		virtual void OnWindowReady() { };
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
