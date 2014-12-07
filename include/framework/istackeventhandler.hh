#pragma once

namespace Framework
{
	class IStackEventHandler
	{
	public:
		virtual void OnStackAdd() = 0;
		virtual void OnStackRemove() = 0;

		virtual void OnStackFocus() = 0;
		virtual void OnStackBlur() = 0;
	};	
}