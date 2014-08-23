#include <stdio.h>

#include "mutex.h"
#include "thread.h"
#include "window.h"

using System::Window;
using System::Thread;
using System::ThreadEntry;

class DisplayThreadEntry : public ThreadEntry {
public:
	virtual ~DisplayThreadEntry() {}

	virtual void *Run(void *arg) {
		Window *mainWindow = Window::Create();
		int exitValue = mainWindow->DoMessageLoop();
		delete mainWindow;
	}
};

int applicationMain()
{
	DisplayThreadEntry displayThreadEntry;
	Thread *displayThread = Thread::Create(&displayThreadEntry);
	displayThread->Start();
	displayThread->Wait();
	return 0;
}
