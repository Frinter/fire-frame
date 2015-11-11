#pragma once

#include "framework/keystate.hh"
#include "system/mousebutton.hh"

namespace Framework {
    class ReadingMouseState
    {
    public:
        virtual int GetMouseX() const = 0;
        virtual int GetMouseY() const = 0;
        virtual KeyState GetMouseButtonState(System::MouseButton mouseButton) = 0;
        virtual int GetScrollDelta() = 0;
    };
}
