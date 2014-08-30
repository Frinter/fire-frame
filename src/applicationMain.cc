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
	SystemWindowController::ReadingKeyboardState *keyboardState = NULL;

	Thread *windowThread = Thread::Create([&windowController, &keyboardState] (void *arg) -> void *
										  {
											  keyboardState = windowController.GetKeyStateReader();
											  windowController.CreateWindow();
										  });

	Thread *timerThread = Thread::Create([&keyboardState] (void *) -> void *
										 {
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
										 });

	windowThread->Start();
	timerThread->Start();
	timerThread->Wait();
	windowThread->Wait();

	return 0;
}
