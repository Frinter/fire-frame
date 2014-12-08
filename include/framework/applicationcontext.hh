#pragma once

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

	private:
		bool m_isClosing;
		bool m_destroyWindowFlag;
	};
}