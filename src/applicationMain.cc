#include <stdio.h>

#include "mutex.hh"
#include "thread.hh"
#include "systemwindowcontroller.hh"

using System::Thread;
using System::ThreadEntry;

class WindowThreadEntry : public ThreadEntry {
public:
	virtual ~WindowThreadEntry() {}

	virtual void *Run(void *arg) {
		windowController.CreateWindow();
	}

private:
	SystemWindowController windowController;
};

int applicationMain()
{
	WindowThreadEntry windowThreadEntry;
	Thread *windowThread = Thread::Create(&windowThreadEntry);
	windowThread->Start();
	windowThread->Wait();
	return 0;
}
