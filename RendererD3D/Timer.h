#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:

	Timer()
	{		
		Reset();
	}

	void Reset()
	{
		QueryPerformanceFrequency(&m_lFrequency);
		QueryPerformanceCounter(&m_lInitialTime);
		m_lLastTime = m_lInitialTime;
	}

	// returns the time in seconds since the timer was initialized or Reset() was last called:
	double GetElapsedTime()
	{
		QueryPerformanceCounter(&m_lCurrentTime);
		return (double)(m_lCurrentTime.QuadPart - m_lInitialTime.QuadPart) / (double)m_lFrequency.QuadPart;
	}

	// returns the time in seconds since GetDeltaTime() was last called (or since initialization if no previous calls):
	double GetDeltaTime()
	{
		QueryPerformanceCounter(&m_lCurrentTime);
		double dDelta = (double)(m_lCurrentTime.QuadPart - m_lLastTime.QuadPart) / (double)m_lFrequency.QuadPart;
		m_lLastTime = m_lCurrentTime;
		return dDelta;
	}

	double PeekDeltaTime()
	{
		QueryPerformanceCounter(&m_lCurrentTime);
		return (double)(m_lCurrentTime.QuadPart - m_lLastTime.QuadPart) / (double)m_lFrequency.QuadPart;		
	}

private:

	LARGE_INTEGER m_lInitialTime;
	LARGE_INTEGER m_lCurrentTime;
	LARGE_INTEGER m_lLastTime;	
	LARGE_INTEGER m_lFrequency;	
};

#endif