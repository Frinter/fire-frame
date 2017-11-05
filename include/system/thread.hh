#pragma once

#include <functional>

namespace System
{
    class Thread
    {
    public:
        typedef std::function<void()> Callable;

        class ThreadImplementation
        {
        public:
            virtual ~ThreadImplementation() {}

            virtual void start() = 0;
            virtual void join() = 0;
        };

    private:
        ThreadImplementation *createImplementation(Callable function);

    public:
        Thread(Callable function);
        ~Thread();

        void join();

    private:
        ThreadImplementation *_implementation;
    };
}
