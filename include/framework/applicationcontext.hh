#pragma once

#include "system/event.hh"

namespace Framework
{
	class ApplicationContext
	{
	public:
		ApplicationContext();

		void Close();
		void SignalWindowDestruction();

		bool IsClosing() const;
		bool ShouldDestroyWindow() const;
		
		System::Event *WindowReady() const;

	private:
		bool m_isClosing;
		bool m_destroyWindowFlag;
		System::Event *m_windowReady;
	};
}