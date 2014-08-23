#include <stdio.h>

#include "mutex.h"
#include "thread.h"
#include "window.h"

using System::Window;

class DisplayThreadEntry : public System::ThreadEntry {
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
	System::Thread *displayThread = System::Thread::Create(&displayThreadEntry);
	displayThread->Start();
	displayThread->Wait();
	return 0;
}
