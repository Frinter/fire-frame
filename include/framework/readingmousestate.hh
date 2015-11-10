#pragma once

namespace Framework {
	class ReadingMouseState
	{
	public:
		virtual int GetMouseX() const = 0;
		virtual int GetMouseY() const = 0;
	};
}
