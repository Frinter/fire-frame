#include <stdio.h>

#include "mutex.hh"
#include "thread.hh"
#include "systemwindowcontroller.hh"

using System::Thread;
using System::ThreadEntry;
using System::SystemWindowController;

class WindowThreadEntry : public ThreadEntry {
public:
	virtual ~WindowThreadEntry() {}

	virtual void *Run(void *arg) {
		keyboardState = windowController.GetKeyStateReader();
		windowController.CreateWindow();
	}

private:
	SystemWindowController windowController;
	SystemWindowController::ReadingKeyboardState *keyboardState;
};

int applicationMain()
{
	WindowThreadEntry windowThreadEntry;
	Thread *windowThread = Thread::Create(&windowThreadEntry);
	windowThread->Start();
	windowThread->Wait();
	return 0;
}
