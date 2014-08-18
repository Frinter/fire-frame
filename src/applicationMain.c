#include "systemMain.h"

void *windowHandle;

int applicationMain()
{
	windowHandle = system_CreateWindow();
	return system_DoMessageLoop();
}
