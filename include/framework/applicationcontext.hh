#pragma once

#include <vector>

#include "system/event.hh"
#include "system/thread.hh"
#include "system/utility.hh"
#include "framework/iwindowcontroller.hh"

namespace System
{
    class Window;
}

namespace Framework
{
    class IApplicationContext
    {
    public:
        virtual IWindowController *createWindowController() = 0;
        virtual System::Window *createWindow(const char *windowName, IWindowController *windowController) = 0;
        virtual System::Utility *GetSystemUtility() const = 0;
    };

    class ApplicationContext : public IApplicationContext
    {
    public:
        ApplicationContext();
        ~ApplicationContext();

        IWindowController *createWindowController();
        System::Window *createWindow(const char *windowName, IWindowController *windowController);
        System::Utility *GetSystemUtility() const;

    private:
        std::vector<System::thread*> m_threads;
    };
}
