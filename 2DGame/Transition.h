#pragma once
#include "Timer.h"
#include "Texture.h"

class Transition final
{
public:
	Transition( float duration );
	~Transition();
	Transition(const Transition& other) = delete;
	Transition& operator=(const Transition& other) = delete;
	Transition(Transition&& other) = delete;
	Transition& operator=(Transition&& other) = delete;

	void Update(float elapsedSec);
	void Draw( Color4f color, Rectf window) const;
	
	void ResetTransition();

	bool IsTransitionComplete() const;
	bool HalfTransition() const;

private:
	float m_Opacity;
	Timer* m_Timer;
};

