
#include "framework/keystate.hh"
#include "framework/readingkeyboardstate.hh"
#include "system/keycode.hh"

namespace Framework
{
	class GameController {
	public:
		GameController();
		virtual ~GameController() {}
		
		void SetKeyboardState(ReadingKeyboardState *state);
		
	protected:
		ReadingKeyboardState *GetKeyboardState();
		
	private:
		ReadingKeyboardState *m_keyboardState;
	};
}