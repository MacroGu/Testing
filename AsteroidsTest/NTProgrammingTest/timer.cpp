
#include "stdafx.h"
#include "timer.h"

#include <time.h>
#include <stdio.h>

Timer::Timer()
: m_TimeDelta(0.f)
, m_Time(0.)
, m_Ticks(0)
{
	Reset();
}

void Timer::Reset()
{
	m_Time = 0.;
	m_TimeDelta = 0.f;
	m_Ticks = clock();
}

void Timer::Update()
{
	long ticks = clock();

	m_TimeDelta = float((double)(ticks - m_Ticks) / CLOCKS_PER_SEC);
	if (m_TimeDelta > 0.2f)
	{
		m_TimeDelta = 0.2f;
	}

	m_Time = m_Time + m_TimeDelta;
	m_Ticks = ticks;
}
