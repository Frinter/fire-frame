#include <cstring>

#include "framework/gamecontroller.hh"

using namespace Framework;

GameController::GameController()
	: m_keyboardState(NULL)
{
}

void GameController::SetKeyboardState(ReadingKeyboardState *state)
{
	m_keyboardState = state;
}

ReadingKeyboardState *GameController::GetKeyboardState()
{
	return m_keyboardState;
}