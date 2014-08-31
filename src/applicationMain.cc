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

int applicationMain()
{
	SystemWindowController windowController;

	ThreadEntry windowThreadEntry = [&windowController] (void*) -> void* {
		windowController.CreateWindow();
	};

	ThreadEntry timerThreadEntry = [&windowController] (void*) -> void* {
		SystemWindowController::ReadingKeyboardState *keyboardState = NULL;
		for (int i = 0; i < 100; ++i) {
			std::cout << "Hello: ";
			keyboardState = windowController.GetKeyStateReader();
			if (keyboardState != NULL) {
				std::cout << keyboardState->GetKeyState(KeyCode::KeyTab);
			}
			else {
				std::cout << "No state";
			}
			std::cout << std::endl;
			Utility::Sleep(1000);
		}
	};

	Thread *windowThread = Thread::Create(windowThreadEntry);
	Thread *timerThread = Thread::Create(timerThreadEntry);

	windowThread->Start();
	timerThread->Start();
	timerThread->Wait();
	windowThread->Wait();

	return 0;
}
