#include <pthread.h>
#include "system/thread.hh"

using System::Thread;

static void *startThread(void *args);

class Implementation : public Thread::ThreadImplementation
{
public:
    Implementation(Thread::Callable function) : _function(function) { }

    void start()
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        _threadHandle = pthread_create(&_threadHandle, &attr, ::startThread, this);
    }

    void join()
    {
        void *result;
        pthread_join(_threadHandle, &result);
    }

    void run()
    {
        _function();
    }

private:
    Thread::Callable _function;
    pthread_t _threadHandle;
};

static void *startThread(void *args)
{
    Implementation *self = (Implementation *)args;
    self->run();
    return NULL;
}

Thread::ThreadImplementation *Thread::createImplementation(Thread::Callable function)
{
    return new Implementation(function);
}

Thread::Thread(Callable function)
{
    _implementation = createImplementation(function);
    _implementation->start();
}

void Thread::join()
{
    _implementation->join();
}
