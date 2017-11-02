#pragma once

#include "system/event.hh"

class LinuxEvent : public System::Event
{
public:
    void Trigger();
    void Wait();
    void Wait(int millisecondTimeout);
};
