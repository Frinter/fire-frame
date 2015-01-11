#pragma once

namespace System
{
	class Utility {
	public:
		virtual void Sleep(int milliseconds) = 0;
		virtual int GetTicks() = 0;
		
		static Utility *GetInstance();
	};
}
