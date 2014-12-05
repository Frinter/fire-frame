#include <cstddef>

#include "framework/gamecontroller.hh"

using namespace Framework;

GameController::GameController()
	: m_keyboardState(NULL), m_controllerStack(NULL)
{
}

void GameController::SetKeyboardState(ReadingKeyboardState *state)
{
	m_keyboardState = state;
}

void GameController::SetControllerStack(IControllerStack *stack)
{
	m_controllerStack = stack;
}

ReadingKeyboardState *GameController::GetKeyboardState()
{
	return m_keyboardState;
}

IControllerStack *GameController::GetControllerStack()
{
	return m_controllerStack;
}
