#pragma once

namespace Framework
{
    class ReadingWindowState
    {
    public:
        virtual void GetSize(unsigned int *width, unsigned int *height) = 0;
    };
}
