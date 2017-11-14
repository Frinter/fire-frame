#define NULL 0

#include "system/utility.hh"

using namespace System;

class LinuxUtility : public Utility
{
public:
    virtual void Sleep(int milliseconds)
    {
    }

    virtual int GetTicks()
    {
        return 0;
    }
};

Utility *Utility::GetInstance()
{
    static Utility *instance = NULL;

    if (instance == NULL)
        instance = new LinuxUtility();

    return instance;
}
