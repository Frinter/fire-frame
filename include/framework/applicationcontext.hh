#pragma once

#include <vector>

#include "framework/iwindowcontroller.hh"
#include "system/thread.hh"
#include "system/utility.hh"

namespace System
{
    class Window;
}

namespace Framework
{
    class IApplicationContext
    {
    public:
        virtual System::Window *createWindow(const char *windowName, IWindowController *windowController) = 0;
        virtual System::Utility *GetSystemUtility() const = 0;
    };

    class ApplicationContext : public IApplicationContext
    {
    public:
        ApplicationContext();
        ~ApplicationContext();

        System::Window *createWindow(const char *windowName, IWindowController *windowController);
        System::Utility *GetSystemUtility() const;

    private:
        std::vector<System::Thread*> m_threads;
    };
}
