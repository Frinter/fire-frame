#pragma once

class Window {
public:
	virtual ~Window() {};
	virtual int DoMessageLoop() = 0;
	static Window *Create();
};

extern void system_AddMenu();
