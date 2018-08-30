#pragma once

#include <stdio.h>

#include "framework/iwindowcontroller.hh"

namespace System
{
    class OpenGLContext;

    class Window {
    public:
        virtual ~Window() {};
        virtual int DoMessageLoop() = 0;
        virtual void GetWindowSize(unsigned int *width, unsigned int *height) = 0;
        virtual bool SetMousePosition(unsigned int posX, unsigned int posY) = 0;
        virtual void Destroy() = 0;
        virtual void Close() = 0;

        virtual void makeBorderlessFullscreen() = 0;
        virtual void makeWindowed() = 0;

        virtual OpenGLContext *getOrCreateOpenGLContext() = 0;

        static Window *Create(const char *name, Framework::ISystemWindowController *controller);
    };
}
