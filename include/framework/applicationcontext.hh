#pragma once

#include "system/event.hh"
#include "framework/applicationstate.hh"

namespace Framework
{
	class ApplicationContext
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

	private:
		bool m_isClosing;
		bool m_destroyWindowFlag;
		ApplicationState *m_applicationState;
		System::Event *m_windowReady;
		System::Event *m_graphicsThreadQuit;
	};
}