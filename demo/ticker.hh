#pragma once

class ISystemTimer
{
public:
	virtual unsigned int GetTicks() = 0;
};

class ISleepService
{
public:
	virtual void Sleep(unsigned int milliseconds) = 0;
};

class Ticker
{
public:
	Ticker(ISystemTimer *systemTimer, ISleepService *sleepService);

	void Start();
	void Wait(unsigned int milliseconds);

private:
	ISystemTimer *m_systemTimer;
	ISleepService *m_sleepService;

	unsigned int m_ticks;
};
