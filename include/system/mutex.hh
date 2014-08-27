#pragma once

namespace System
{
	class Mutex {
	public:
		virtual ~Mutex() {}

		virtual void Lock() = 0;
		virtual void Unlock() = 0;

		static Mutex *Create();
	};
}
