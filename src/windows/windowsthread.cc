#include <windows.h>
#include "system/thread.hh"

using System::Thread;

DWORD WINAPI startThread(LPVOID args);

class Implementation : public Thread::ThreadImplementation
{
public:
    Implementation(Thread::Callable function) : _function(function) { }

    void start()
    {
        _threadHandle = CreateThread(NULL, 0, ::startThread, this, 0, NULL);
    }

    void join()
    {
        WaitForSingleObject(_threadHandle, INFINITE);
    }

    void run()
    {
        _function();
    }

private:
    Thread::Callable _function;
    HANDLE _threadHandle;
};

DWORD WINAPI startThread(LPVOID args)
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
