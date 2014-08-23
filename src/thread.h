#pragma once

namespace System
{
	class ThreadEntry {
	public:
		virtual ~ThreadEntry() {}

		virtual void *Run(void *) = 0;
	};

	class Thread {
	public:
		Thread(const ThreadEntry *entry) {}

		virtual ~Thread() {}

		virtual void Start() = 0;
		virtual void Wait() = 0;

		static Thread *Create(const ThreadEntry *entry);
	};
}
