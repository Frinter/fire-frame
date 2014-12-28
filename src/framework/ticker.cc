#include <iostream>

#include "framework/ticker.hh"

Framework::Ticker::Ticker(Framework::ISystemTimer *systemTimer, Framework::ISleepService *sleepService)
{
	m_systemTimer = systemTimer;
	m_sleepService = sleepService;
}

void Framework::Ticker::Start()
{
	m_ticks = m_systemTimer->GetTicks();
}

void Framework::Ticker::Wait(unsigned int milliseconds)
{
	unsigned int newTicks = m_systemTimer->GetTicks();
	unsigned int difference = newTicks - m_ticks;
	
	if (difference <= milliseconds)
	{
		unsigned int sleepTime = milliseconds - difference;
		m_sleepService->Sleep(sleepTime);
		m_ticks += sleepTime;
	}
	else
	{
		m_ticks += milliseconds;
	}
}