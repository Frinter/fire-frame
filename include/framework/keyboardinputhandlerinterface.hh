#pragma once

#include "system/keycode.hh"

namespace Framework {
    class KeyboardInputHandlerInterface
    {
    public:
        virtual ~KeyboardInputHandlerInterface() {}

        virtual void PressKey(System::KeyCode key) = 0;
        virtual void UnpressKey(System::KeyCode key) = 0;
    };
}
