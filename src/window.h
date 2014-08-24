#pragma once

#include <stdio.h>

namespace System
{
	class WindowController {
	public:
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
