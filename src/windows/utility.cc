#include <windows.h>

#include "system/utility.hh"

using namespace System;

class WindowsUtility : public Utility
{
public:
    virtual void Sleep(int milliseconds)
    {
        ::Sleep(milliseconds);
    }

    virtual unsigned long GetTicks()
    {
        return timeGetTime();
    }
};

Utility *Utility::GetInstance()
{
    static Utility *instance = NULL;

    if (instance == NULL)
        instance = new WindowsUtility();

    return instance;
}
