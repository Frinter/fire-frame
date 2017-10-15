#pragma once

#include <vector>

#include "system/event.hh"
#include "system/thread.hh"
#include "system/utility.hh"
#include "framework/iwindowcontroller.hh"

namespace Framework
{
    class IApplicationContext
    {
    public:
        virtual IWindowController *createWindow(const char *windowName) = 0;
        virtual System::Utility *GetSystemUtility() const = 0;
    };

    class ApplicationContext : public IApplicationContext
    {
    public:
        ApplicationContext();
        ~ApplicationContext();

        IWindowController *createWindow(const char *windowName);
        System::Utility *GetSystemUtility() const;

    private:
        std::vector<System::thread*> m_threads;
    };
}
