#include <pthread.h>
#include "system/mutex.hh"

class LinuxMutex : public System::Mutex
{
public:
    LinuxMutex()
    {
        pthread_mutex_init(&_handle, NULL);
    }

    ~LinuxMutex()
    {
        pthread_mutex_destroy(&_handle);
    }

    void Lock()
    {
        pthread_mutex_lock(&_handle);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_handle);
    }

private:
    pthread_mutex_t _handle;
};

System::Mutex *System::Mutex::Create()
{
    return new LinuxMutex();
}
