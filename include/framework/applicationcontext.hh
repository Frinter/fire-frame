#pragma once

#include <vector>

#include "system/event.hh"
#include "system/thread.hh"
#include "system/utility.hh"
#include "framework/applicationstate.hh"
#include "framework/iwindowcontroller.hh"

namespace Framework
{
    class IApplicationContext
    {
    public:
        virtual void Close() = 0;
        virtual bool IsClosing() const = 0;
        virtual System::Utility *GetSystemUtility() const = 0;
        virtual IWindowController *createWindow() = 0;
    };

    class ApplicationContext : public IApplicationContext
    {
    public:
        ApplicationContext(ApplicationState *applicationState);
        ~ApplicationContext();

        void Close();
        void SignalWindowDestruction();
        IWindowController *createWindow();

        bool IsClosing() const;
        bool ShouldDestroyWindow() const;

        System::Event *WindowReady() const;
        System::Event *ApplicationThreadQuit() const;

        ApplicationState *GetState() const;
        System::Utility *GetSystemUtility() const;

    private:
        bool m_isClosing;
        bool m_destroyWindowFlag;
        ApplicationState *m_applicationState;
        System::Event *m_windowReady;
        System::Event *m_applicationThreadQuit;
        std::vector<System::thread*> m_threads;
    };
}
