#pragma once

#include "window.hh"

namespace System
{
	class OpenGLContext {
	public:
		virtual ~OpenGLContext() {};

		virtual void SwapBuffers() = 0;
		
		static OpenGLContext *Create(Window *window);
	};
}
