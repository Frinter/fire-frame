#pragma once

#include "window.hh"

namespace System
{
	class OpenGLContext {
	public:
		virtual ~OpenGLContext() {};

		static OpenGLContext *Create(Window *window);
	};
}
