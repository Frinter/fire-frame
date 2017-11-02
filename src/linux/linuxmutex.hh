#pragma once

#include "system/mutex.hh"

class LinuxMutex : public System::Mutex
{
public:
    void Lock();
    void Unlock();
};
