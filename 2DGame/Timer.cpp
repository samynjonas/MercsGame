#include "pch.h"
#include "Timer.h"
Timer::Timer()
	: m_TimeGoal{ 0.f }
	, m_AccumulateSeconds{ 0.f }
	, m_IsTimerDone{ false }
{

}

void Timer::Update(float elapsedSec)
{
	if (m_IsTimerDone == false)
	{
		m_AccumulateSeconds += elapsedSec;
		if (m_AccumulateSeconds >= m_TimeGoal)
		{
			m_IsTimerDone = true;
		}
	}
}

void Timer::setTimeGoal(float seconds)
{
	m_TimeGoal = seconds;
	resetTimer();
}

void Timer::resetTimer()
{
	m_AccumulateSeconds = 0;
	m_IsTimerDone = false;
}

bool Timer::isTimerDone() const
{
	return m_IsTimerDone;
}

float Timer::getCurrentTime() const
{
	return m_AccumulateSeconds;
}

float Timer::getTimeGoal() const
{
	return m_TimeGoal;
}