#pragma once

#include <windows.h>

#include "system/window.hh"

class WindowsOpenGLContext;

class WindowsWindow : public System::Window
{
    friend class WindowsOpenGLContext;

public:
    WindowsWindow(
        const char *windowName,
        Framework::ISystemWindowController *controller,
        HINSTANCE processInstance,
        int commandShow
    );

    virtual ~WindowsWindow();
    virtual void GetWindowSize(unsigned int *width, unsigned int *height);
    virtual bool SetMousePosition(unsigned int posX, unsigned int posY);

    virtual System::OpenGLContext *getOrCreateOpenGLContext();

    virtual int DoMessageLoop();

    virtual void Destroy();
    virtual void Close();

private:
    HWND m_windowHandle;
    Framework::ISystemWindowController *m_controller;
    WindowsOpenGLContext *m_openGLContext;

    void Ready();

    void onClose();
    void WindowResize(WPARAM wParam, LPARAM lParam);
    void KeyDown(WPARAM key);
    void KeyUp(WPARAM key);
    void MouseMove(WPARAM wParam, LPARAM lParam);
    void LeftMouseButtonDown(WPARAM wParam, LPARAM lParam);
    void RightMouseButtonDown(WPARAM wParam, LPARAM lParam);
    void LeftMouseButtonUp(WPARAM wParam, LPARAM lParam);
    void RightMouseButtonUp(WPARAM wParam, LPARAM lParam);
    void MouseScrollWheel(WPARAM wParam, LPARAM lParam);

    WindowsWindow(const WindowsWindow &o) = delete;

    static HWND makeWindow(const char *name, HINSTANCE processInstance, int commandShow);
    static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wparam, LPARAM lparam);
};
