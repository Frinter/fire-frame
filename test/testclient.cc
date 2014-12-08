#include <iostream>
#include "framework/igraphicsthreadcontroller.hh"

class GraphicsThreadController : public Framework::IGraphicsThreadController
{
public:
	void Run()
	{
		std::cout << "Graphics thread running" << std::endl;
	}
};

Framework::IGraphicsThreadController *GetGraphicsThreadController()
{
	// memory leak
	return new GraphicsThreadController();
}
