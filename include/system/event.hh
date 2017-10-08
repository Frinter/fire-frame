#pragma once

namespace System
{
    class Event
    {
    public:
        virtual ~Event() {}

        Event() {}
        Event(const Event &o) = delete;

        virtual void Trigger() = 0;
        virtual void Wait() = 0;
        virtual void Wait(int millisecondTimeout) = 0;

        static Event *Create(const char *name);
        static Event *Create();
    };
}
