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

class OtherTestController : public GameController {
public:
	virtual void OnStackAdd()
	{
		std::cout << "OtherTestController added to stack" << std::endl;
	}

	virtual void OnStackRemove()
	{
		std::cout << "OtherTestController removed from stack" << std::endl;
	}	
};

class TestController : public GameController {
public:
	TestController()
		: m_otherController(NULL)
	{
	}
	
	~TestController()
	{
		if (m_otherController != NULL)
		{
			GetControllerStack()->Pop();
			delete m_otherController;
			m_otherController = NULL;			
		}
	}
	
	virtual void OnStackAdd()
	{
		std::cout << "TestController added to stack" << std::endl;
	}

	virtual void OnStackRemove()
	{
		std::cout << "TestController removed from stack" << std::endl;
	}
	
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

		if (m_otherController == NULL)
		{
			m_otherController = new OtherTestController();
			GetControllerStack()->Push(m_otherController);
		}
		else
		{
			GetControllerStack()->Pop();
			delete m_otherController;
			m_otherController = NULL;
		}
	}
	
private:
	OtherTestController *m_otherController;
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
		
		controllerStack.Pop();
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
