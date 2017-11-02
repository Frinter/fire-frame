#include "linuxmutex.hh"

System::Mutex *System::Mutex::Create()
{
    return new LinuxMutex();
}

void LinuxMutex::Lock()
{
}

void LinuxMutex::Unlock()
{
}
