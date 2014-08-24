#include <stdexcept>
#include <windows.h>

#include "../thread.hh"

using System::ThreadEntry;
using System::Thread;

DWORD WINAPI threadEntryFunction(LPVOID param)
{
	((ThreadEntry *)param)->Run(NULL);
}

class WindowsThread : public Thread {
public:
	WindowsThread(const ThreadEntry *entry) 
		: Thread(entry), m_entry(entry), m_handle(NULL)
	{ }

	virtual ~WindowsThread() {
	}

	virtual void Start() {
		m_handle = CreateThread(NULL, 0, threadEntryFunction, (LPVOID)m_entry, 0, &m_threadId);
	}

	virtual void Wait() {
		if (m_handle != NULL)
		{
			WaitForSingleObject(m_handle, INFINITE);
			CloseHandle(m_handle);
			m_handle = NULL;
		}
	}

private:
	const ThreadEntry *m_entry;
	HANDLE m_handle;
	DWORD m_threadId;

	WindowsThread(const WindowsThread &o) : Thread(NULL) {
		throw new std::logic_error("WindowsThread copy contructor called");
	}
};

Thread *Thread::Create(const ThreadEntry *entry)
{
	return new WindowsThread(entry);
}
