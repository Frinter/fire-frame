#include <windows.h>

#include "system/event.hh"

class WindowsEvent : public System::Event
{
public:
	WindowsEvent(const char *name)
	{
		m_eventHandle = CreateEvent(NULL, true, false, name);
		ResetEvent(m_eventHandle);
	}
		
	virtual void Trigger()
	{
		SetEvent(m_eventHandle);
	}
	
	virtual void Wait()
	{
		WaitForSingleObject(m_eventHandle, INFINITE);
	}
	
	virtual void Wait(int millisecondTimeout)
	{
		WaitForSingleObject(m_eventHandle, millisecondTimeout);
	}

	private:
	HANDLE m_eventHandle;
};

System::Event *System::Event::Create(const char *name)
{
	return new WindowsEvent(name);
}