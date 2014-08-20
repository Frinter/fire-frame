#include "systemMain.h"

int applicationMain()
{
	Window windowHandle = system_CreateWindow();
	// fileMenu = system_addMenu(windowHandle, "File")
	return system_DoMessageLoop();
}
