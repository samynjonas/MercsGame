#include "pch.h"
#include "Transition.h"


Transition::Transition( float duration )
	: m_Opacity{ 0 }
	, m_Timer{ new Timer{} }
{
	m_Timer->setTimeGoal( duration );
}

Transition::~Transition()
{
	delete m_Timer;
	m_Timer = nullptr;
}

void Transition::Update(float elapsedSec)
{
	m_Timer->Update(elapsedSec);

	if (m_Timer->getCurrentTime() <= m_Timer->getTimeGoal() / 2)
	{
		m_Opacity += 10;
	}
	else
	{
		m_Opacity -= 10;
	}
}


void Transition::Draw(Color4f color, Rectf window) const
{
	color.a = (m_Opacity / 255.f);
	utils::SetColor(color);
	utils::FillRect(window);
}

bool Transition::IsTransitionComplete() const
{
	return m_Timer->isTimerDone();
}
bool Transition::HalfTransition() const
{
	if (m_Timer->getCurrentTime() >= m_Timer->getTimeGoal() / 2)
	{
		return true;
	}
	return false;
}

void Transition::ResetTransition()
{
	m_Opacity = 0;
	m_Timer->resetTimer();
}