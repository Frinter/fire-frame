#include <windows.h>

#include "system/thread.hh"

using namespace System;

DWORD WINAPI StartThread(LPVOID args)
{
	thread::IThreadImplementation *self = (thread::IThreadImplementation *)args;
	self->Run();
	return NULL;
}

void thread::StartThread()
{
	PDWORD threadId;
	CreateThread(NULL, 0, ::StartThread, this->m_implementation, 0, threadId);
}
