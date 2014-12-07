#pragma once

namespace Framework
{
	class ApplicationContext
	{
	public:
		ApplicationContext();

		void Close();

		bool IsClosing() const;

	private:
		bool m_isClosing;
	};
}