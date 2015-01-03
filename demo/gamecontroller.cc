#include <cstddef>

#include "gamecontroller.hh"

GameController::GameController()
	: m_keyboardState(NULL), m_controllerStack(NULL)
{
}

void GameController::SetKeyboardState(Framework::ReadingKeyboardState *state)
{
	m_keyboardState = state;
}

void GameController::SetControllerStack(IControllerStack *stack)
{
	m_controllerStack = stack;
}

Framework::ReadingKeyboardState *GameController::GetKeyboardState()
{
	return m_keyboardState;
}

IControllerStack *GameController::GetControllerStack()
{
	return m_controllerStack;
}
