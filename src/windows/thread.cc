#include <functional>
#include <stdexcept>
#include <windows.h>

#include "system/thread.hh"

using System::ThreadEntry;
using System::Thread;

struct WindowsThreadEntryArguments {
	ThreadEntry entryFunction;
	void *arguments;
};

DWORD WINAPI threadEntryFunction(LPVOID param)
{
	WindowsThreadEntryArguments *arguments = static_cast<WindowsThreadEntryArguments*>(param);
	(arguments->entryFunction).operator()(arguments->arguments);
	delete arguments;
}

class WindowsThread : public Thread {
public:
	WindowsThread(const ThreadEntry entry, void *params = NULL) 
		: Thread(entry), m_entry(entry), m_handle(NULL), m_param(params)
	{ }

	virtual ~WindowsThread() {
	}

	virtual void Start() {
		WindowsThreadEntryArguments *arguments = new WindowsThreadEntryArguments();
		arguments->entryFunction = m_entry;
		arguments->arguments = m_param;
		m_handle = CreateThread(NULL, 0, threadEntryFunction, (LPVOID)arguments, 0, &m_threadId);
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
	const ThreadEntry m_entry;
	HANDLE m_handle;
	DWORD m_threadId;
	void *m_param;

	WindowsThread(const WindowsThread &o) = delete;
	WindowsThread(WindowsThread &&o) = delete;
};

Thread *Thread::Create(const ThreadEntry entry, void *params)
{
	return new WindowsThread(entry, params);
}


