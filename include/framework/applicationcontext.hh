#pragma once

#include "system/event.hh"
#include "system/utility.hh"
#include "framework/applicationstate.hh"

namespace Framework
{
	class IApplicationContext
	{
	public:
		virtual bool IsClosing() const = 0;
		virtual System::Utility *GetSystemUtility() const = 0;
	};
	
	class ApplicationContext : public IApplicationContext
	{
	public:
		ApplicationContext(ApplicationState *applicationState);

		void Close();
		void SignalWindowDestruction();

		bool IsClosing() const;
		bool ShouldDestroyWindow() const;
		
		System::Event *WindowReady() const;
		System::Event *GraphicsThreadQuit() const;

		ApplicationState *GetState() const;
		System::Utility *GetSystemUtility() const;

	private:
		bool m_isClosing;
		bool m_destroyWindowFlag;
		ApplicationState *m_applicationState;
		System::Event *m_windowReady;
		System::Event *m_graphicsThreadQuit;
	};
}