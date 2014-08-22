#pragma once

namespace System
{
	class Window {
	public:
		virtual ~Window() {};
		virtual int DoMessageLoop() = 0;
		static Window *Create();
	};
}
