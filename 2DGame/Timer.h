#pragma once
class Timer
{
public:
	Timer();

	void Update(float elapsedSec);

	void setTimeGoal( float seconds );
	void resetTimer();
	float getCurrentTime() const;

	bool isTimerDone() const;
	float getTimeGoal() const;

private:
	float m_TimeGoal;
	float m_AccumulateSeconds;

	bool m_IsTimerDone;
};

