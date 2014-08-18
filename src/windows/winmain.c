#include <windows.h>

extern int applicationMain();

static char appName[] = "Test";

HINSTANCE windows_instance;
HINSTANCE windows_parentInstance;
LPSTR windows_arguments;
int windows_commandShow;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE parentInstance, LPSTR arguments, int commandShow)
{
	windows_instance = instance;
	windows_parentInstance = parentInstance;
	windows_arguments = arguments;
	windows_commandShow = commandShow;

	return applicationMain();
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(message)
	{
	case WM_PAINT:
		hdc = BeginPaint(windowHandle, &ps);
		TextOut(hdc, 100, 100, "Hello world", 11);
		EndPaint(windowHandle, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wparam, lparam);
}

void *system_CreateWindow()
{
	HWND windowHandle;
	WNDCLASSEX windowClass;

	windowClass.cbSize        = sizeof(WNDCLASSEX);
	windowClass.style         = CS_VREDRAW | CS_HREDRAW;
	windowClass.lpfnWndProc   = WndProc;
	windowClass.cbClsExtra    = 0;
	windowClass.cbWndExtra    = 0;
	windowClass.hInstance     = windows_instance;
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	windowClass.lpszClassName = appName;
	windowClass.lpszMenuName  = NULL;

	RegisterClassEx(&windowClass);

	windowHandle = CreateWindow(appName, "Hello world", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, windows_instance, NULL);

	ShowWindow(windowHandle, windows_commandShow);
	UpdateWindow(windowHandle);
}

int system_DoMessageLoop()
{
	MSG message;

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}
