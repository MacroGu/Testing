//-------------------------------------------------------------------------------------------------------------
// timer.h
//
// Created: JohnL
//
// A general-purpose timer.
//
// Copyright(c) Ninja Theory 2010. All Rights Reserved.
//-------------------------------------------------------------------------------------------------------------

#pragma once

class Timer
{
public:
	Timer();

	void Reset();
	void Update();

	float GetTimeDelta() { return m_TimeDelta; }
	double GetTime()     { return m_Time; }

private:
	float           m_TimeDelta;
	double          m_Time;
	unsigned long   m_Ticks;
};
