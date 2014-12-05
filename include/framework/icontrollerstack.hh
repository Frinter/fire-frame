#pragma once

namespace Framework
{
	class GameController;
	
	class IControllerStack
	{
	public:
		virtual void Push(GameController *controller) = 0;
		virtual void Pop() = 0;
	};
}