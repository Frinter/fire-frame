#pragma once

namespace System
{
	class Window {
	public:
		virtual ~Window() {};
		virtual int DoMessageLoop() = 0;
		static Window *Create();
	};

	class OpenGLContext {
	public:
		virtual ~OpenGLContext() {};

		static OpenGLContext *Create(Window *window);
	};
}
