#include <windows.h>

#include "system/thread.hh"

using namespace System;

DWORD WINAPI StartThread(LPVOID args);

class ThreadImplementation : public thread::IThreadImplementation
{
public:
    ThreadImplementation(thread::Callable function) : _function(function) {
    }

    virtual void start() {
        _threadHandle = CreateThread(NULL, 0, ::StartThread, this, 0, NULL);
    }

    virtual void join() {
        WaitForSingleObject(_threadHandle, INFINITE);
    }

    void run() {
        _function();
    }

private:
    thread::Callable _function;
    HANDLE _threadHandle;
};

thread::IThreadImplementation *thread::createImplementation(Callable function)
{
    return new ThreadImplementation(function);
}

thread::thread(Callable function)
    : _implementation(NULL)
{
    _implementation = createImplementation(function);
    _implementation->start();
}

thread::~thread()
{
    if (_implementation != NULL)
        delete _implementation;
}

void thread::join()
{
    _implementation->join();
}

DWORD WINAPI StartThread(LPVOID args)
{
    ThreadImplementation *self = (ThreadImplementation *)args;
    self->run();
    return NULL;
}
