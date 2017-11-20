#include <poll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "system/event.hh"

class LinuxEvent : public System::Event
{
public:
    LinuxEvent()
    {
        _handle = eventfd(0, 0);
    }

    ~LinuxEvent()
    {
        close(_handle);
    }

    void Trigger()
    {
        eventfd_write(_handle, 1);
    }

    void Wait()
    {
        Wait(-1);
    }

    void Wait(int millisecondTimeout)
    {
        struct pollfd info;
        info.fd = _handle;
        info.events = POLLIN;
        info.revents = 0;

        poll(&info, 1, millisecondTimeout);

        eventfd_t buffer;
        eventfd_read(_handle, &buffer);
    }

private:
    int _handle;
};

System::Event *System::Event::Create(const char *name)
{
    return new LinuxEvent();
}

System::Event *System::Event::Create()
{
    return new LinuxEvent();
}
