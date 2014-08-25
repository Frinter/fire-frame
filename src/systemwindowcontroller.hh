#pragma once

#include "keycode.hh"
#include "window.hh"
#include "openglcontext.hh"

class SystemWindowController : public System::WindowController
{
public:
	SystemWindowController();

	virtual ~SystemWindowController();

	void CreateWindow();

	virtual void OnWindowReady();
	virtual void OnKeyDown(System::KeyCode key);
	virtual void OnKeyUp(System::KeyCode key);

private:
	System::Window *m_window;
	System::OpenGLContext *m_openGLContext;
};
