#include <iostream>

#include "system/mutex.hh"
#include "system/thread.hh"
#include "system/systemwindowcontroller.hh"
#include "system/systemutility.hh"

using System::Thread;
using System::ThreadEntry;
using System::SystemWindowController;
using System::Utility;
using System::KeyCode;

SystemWindowController::ReadingKeyboardState *keyboardState = NULL;

class WindowThreadEntry : public ThreadEntry {
public:
	virtual ~WindowThreadEntry() {}

	virtual void *Run(void *arg) {
		keyboardState = windowController.GetKeyStateReader();
		windowController.CreateWindow();
	}

private:
	SystemWindowController windowController;
};

class TimerThreadEntry : public ThreadEntry {
public:
	virtual ~TimerThreadEntry() {}

	virtual void *Run(void *arg) {
		for (int i = 0; i < 100; ++i) {
			std::cout << "Hello: ";
			if (keyboardState != NULL) {
				std::cout << keyboardState->GetKeyState(KeyCode::KeyTab);
			}
			else {
				std::cout << "No state";
			}
			std::cout << std::endl;
			Utility::Sleep(1000);
		}
	}
};

int applicationMain()
{
	WindowThreadEntry windowThreadEntry;
	TimerThreadEntry timerThreadEntry;

	Thread *windowThread = Thread::Create(&windowThreadEntry);
	Thread *timerThread = Thread::Create(&timerThreadEntry);
	windowThread->Start();
	timerThread->Start();
	timerThread->Wait();
	windowThread->Wait();
	return 0;
}
