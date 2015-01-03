#include <iostream>

#include "ticker.hh"

Ticker::Ticker(ISystemTimer *systemTimer, ISleepService *sleepService)
{
	m_systemTimer = systemTimer;
	m_sleepService = sleepService;
}

void Ticker::Start()
{
	m_ticks = m_systemTimer->GetTicks();
}

void Ticker::Wait(unsigned int milliseconds)
{
	unsigned int currentTicks = m_systemTimer->GetTicks();
	unsigned int targetTicks = m_ticks + milliseconds;
	
	if (targetTicks > currentTicks)
	{
		unsigned int sleepTime = targetTicks - currentTicks;
		m_sleepService->Sleep(sleepTime);
		m_ticks = targetTicks;
	}
	else
	{
		m_ticks = targetTicks;
	}
}