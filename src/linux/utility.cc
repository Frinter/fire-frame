#include <time.h>
#include <unistd.h>

#include "system/utility.hh"

using namespace System;

class LinuxUtility : public Utility
{
public:
    virtual void Sleep(int milliseconds)
    {
        ::usleep(1000 * milliseconds);
    }

    virtual unsigned long GetTicks()
    {
        struct timespec result;
        clock_gettime(CLOCK_REALTIME, &result);
        return (1000 * result.tv_sec) + (result.tv_nsec / 1.0e6);
    }
};

Utility *Utility::GetInstance()
{
    static Utility *instance = NULL;

    if (instance == NULL)
        instance = new LinuxUtility();

    return instance;
}
