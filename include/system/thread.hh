#pragma once

#include <functional>

namespace System
{
    class thread
    {
    public:
        typedef std::function<void()> Callable;

        class IThreadImplementation
        {
        public:
            virtual ~IThreadImplementation() {}
            
            virtual void start() = 0;
            virtual void join() = 0;
        };

    private:
        IThreadImplementation *createImplementation(Callable function);
        
    public:
        thread(Callable function);
        ~thread();
		
        void join();
		
    private:
        IThreadImplementation *_implementation;
    };
}
