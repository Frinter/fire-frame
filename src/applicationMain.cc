#include <iostream>

#include "system/mutex.hh"
#include "system/thread.hh"
#include "system/systemutility.hh"
#include "framework/controllerstack.hh"
#include "framework/gamecontroller.hh"
#include "framework/windowcontroller.hh"

using System::Thread;
using System::ThreadEntry;
using System::Utility;
using System::KeyCode;
using Framework::ControllerStack;
using Framework::GameController;
using Framework::ReadingKeyboardState;
using Framework::WindowController;

class TestController : public GameController {
public:
	void Check()
	{
		ReadingKeyboardState *keyboardState = GetKeyboardState();

		std::cout << "TestController: ";
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
	ControllerStack controllerStack(&windowController);

	ThreadEntry windowThreadEntry = [&windowController] (void*) -> void* {
		windowController.CreateWindow();
	};

	ThreadEntry timerThreadEntry = [&controllerStack] (void*) -> void* {
		TestController *controller = new TestController();
		
		controllerStack.Push(controller);
		
		for (int i = 0; i < 15; ++i) {
			controller->Check();

			Utility::Sleep(1000);
		}
		
		delete controller;
	};

	Thread *windowThread = Thread::Create(windowThreadEntry);
	Thread *timerThread = Thread::Create(timerThreadEntry);

	windowThread->Start();
	timerThread->Start();
	timerThread->Wait();
	windowThread->Wait();

	return 0;
}
