#pragma once

#include <functional>

namespace System
{
	typedef std::function<void* (void*)> ThreadEntry;

	class Thread {
	public:
		Thread(const ThreadEntry entry) {}

		virtual ~Thread() {}

		virtual void Start() = 0;
		virtual void Wait() = 0;

		static Thread *Create(const ThreadEntry entry);
	};
}
