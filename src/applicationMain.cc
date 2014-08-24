#include <stdio.h>

#include "mutex.hh"
#include "thread.hh"
#include "window.hh"

using System::Window;
using System::Thread;
using System::ThreadEntry;
using System::OpenGLContext;
using System::WindowController;
using System::KeyCode;

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

	virtual void OnKeyDown(KeyCode key)
	{
		printf("Key Down\n");
	}

	virtual void OnKeyUp(KeyCode key)
	{
		printf("Key Up\n");
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
