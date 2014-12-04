
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

namespace Framework
{
	class GameController {
	public:
		GameController();
		
		virtual void SetKeyboardState() = 0;
	};
}