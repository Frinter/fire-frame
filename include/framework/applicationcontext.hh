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
        virtual System::Utility *GetSystemUtility() const = 0;
        virtual IWindowController *createWindow(const char *windowName) = 0;
    };

    class ApplicationContext : public IApplicationContext
    {
    public:
        ApplicationContext();
        ~ApplicationContext();

        void SignalWindowDestruction();
        IWindowController *createWindow(const char *windowName);

        bool ShouldDestroyWindow() const;

        System::Event *ApplicationThreadQuit() const;

        System::Utility *GetSystemUtility() const;

    private:
        bool m_destroyWindowFlag;
        System::Event *m_applicationThreadQuit;
        std::vector<System::thread*> m_threads;
    };
}
