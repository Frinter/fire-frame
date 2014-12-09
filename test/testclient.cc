#include <iostream>

#include "framework/gamecontroller.hh"
#include "framework/igraphicsthreadcontroller.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

using Framework::ReadingKeyboardState;
using System::KeyCode;

class GraphicsThreadController : public Framework::IGraphicsThreadController
{
public:
	void Run()
	{
		std::cout << "Graphics thread running" << std::endl;
	}
};

class OtherTestController : public Framework::GameController {
public:	
	virtual void OnStackAdd()
	{
		std::cout << "OtherTestController added to stack" << std::endl;
	}

	virtual void OnStackRemove()
	{
		std::cout << "OtherTestController removed from stack" << std::endl;
	}	

	virtual void OnStackFocus()
	{
		std::cout << "OtherTestController stack focus" << std::endl;
	}

	virtual void OnStackBlur()
	{
		std::cout << "OtherTestController stack blur" << std::endl;
	}
};

class TestController : public Framework::GameController {
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
	
	virtual void OnStackFocus()
	{
		std::cout << "TestController stack focus" << std::endl;
	}

	virtual void OnStackBlur()
	{
		std::cout << "TestController stack blur" << std::endl;
	}

	virtual void OnTick()
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

Framework::IGraphicsThreadController *GetGraphicsThreadController()
{
	// memory leak
	return new GraphicsThreadController();
}

Framework::GameController *GetBaseController()
{
	return new TestController();
}
