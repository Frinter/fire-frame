#include <windows.h>

#include "system/utility.hh"

using namespace System;

void Utility::Sleep(int milliseconds)
{
	::Sleep(milliseconds);
}
