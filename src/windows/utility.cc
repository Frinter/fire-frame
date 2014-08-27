#include <windows.h>

#include "system/systemutility.hh"

using namespace System;

void Utility::Sleep(int milliseconds)
{
	::Sleep(milliseconds);
}
