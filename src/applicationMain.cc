#include "systemMain.h"

int applicationMain()
{
	Window *mainWindow = Window::Create();
	// fileMenu = system_addMenu(windowHandle, "File")
	int exitValue = mainWindow->DoMessageLoop();

	delete mainWindow;

	return exitValue;
}
