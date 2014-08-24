#include <stdio.h>

#include "mutex.h"
#include "thread.h"
#include "window.h"

using System::Window;
using System::Thread;
using System::ThreadEntry;
using System::OpenGLContext;
using System::WindowController;

class TestWindowController : public WindowController
{
public:
	TestWindowController() : m_window(NULL)
	{ }

	~TestWindowController()
	{
		if (m_window != NULL)
			delete m_window;
	}

	void CreateWindow()
	{
		m_window = Window::Create(this);
		m_window->DoMessageLoop();
	}

	virtual void OnWindowReady()
	{
		m_openGLContext = OpenGLContext::Create(m_window);
	}
private:
	Window *m_window;
	OpenGLContext *m_openGLContext;
};

class WindowThreadEntry : public ThreadEntry {
public:
	virtual ~WindowThreadEntry() {}

	virtual void *Run(void *arg) {
		windowController.CreateWindow();
	}

private:
	TestWindowController windowController;
};

int applicationMain()
{
	WindowThreadEntry windowThreadEntry;
	Thread *windowThread = Thread::Create(&windowThreadEntry);
	windowThread->Start();
	windowThread->Wait();
	return 0;
}
