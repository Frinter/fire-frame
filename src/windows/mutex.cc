#include <stdexcept>
#include <windows.h>

#include "system/mutex.hh"

using System::Mutex;

class WindowsMutex : public Mutex {
public:
	WindowsMutex() {
		inCriticalSection = false;
		InitializeCriticalSection(&m_criticalSection);
	}

	virtual ~WindowsMutex() {
		DeleteCriticalSection(&m_criticalSection);
	}

	virtual void Lock() {
		EnterCriticalSection(&m_criticalSection);
		inCriticalSection = true;
	}

	virtual void Unlock() {
		if (inCriticalSection)
		{
			inCriticalSection = false;
			LeaveCriticalSection(&m_criticalSection);
		}
	}

private:
	CRITICAL_SECTION m_criticalSection;
	bool inCriticalSection;

	WindowsMutex(const WindowsMutex &o) = delete;
};

Mutex *Mutex::Create()
{
	return new WindowsMutex();
}
