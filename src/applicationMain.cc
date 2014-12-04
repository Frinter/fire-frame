#include <iostream>

#include "system/mutex.hh"
#include "system/thread.hh"
#include "system/systemutility.hh"
#include "framework/gamecontroller.hh"
#include "framework/windowcontroller.hh"

using System::Thread;
using System::ThreadEntry;
using System::Utility;
using System::KeyCode;
using Framework::WindowController;

class TestController : public Framework::GameController {
public:
	void Check()
	{
		Framework::ReadingKeyboardState *keyboardState = GetKeyboardState();

		std::cout << "Hello: ";
		if (keyboardState != NULL) {
			std::cout << keyboardState->GetKeyState(KeyCode::KeyTab);
		}
		else {
			std::cout << "No state";
		}
		std::cout << std::endl;
	}
};

int applicationMain()
{
	WindowController windowController;

	ThreadEntry windowThreadEntry = [&windowController] (void*) -> void* {
		windowController.CreateWindow();
	};

	ThreadEntry timerThreadEntry = [&windowController] (void*) -> void* {
		TestController controller;
		for (int i = 0; i < 15; ++i) {
			controller.SetKeyboardState(windowController.GetKeyStateReader());
			controller.Check();

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
