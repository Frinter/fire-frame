#include <windows.h>

#include "../systemutility.hh"

using namespace System;

void Utility::Sleep(int milliseconds)
{
	::Sleep(milliseconds);
}
