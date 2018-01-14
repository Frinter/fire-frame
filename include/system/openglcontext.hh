#pragma once

#include "window.hh"

namespace System
{
    class OpenGLContext {
    public:
        virtual void SwapBuffers() = 0;

    protected:
        virtual ~OpenGLContext() {};
    };
}
