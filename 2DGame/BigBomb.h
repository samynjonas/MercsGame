#pragma once
#include "Bullet.h"
#include "ParticleSystem.h"
#include "Timer.h"

class BigBomb final : public Bullet
{
public:
	BigBomb(Point2f startPoint, Vector2f direction, ResourceManager* resourceManager);
	virtual ~BigBomb();

	BigBomb(const BigBomb& other) = delete;
	BigBomb& operator=(const BigBomb& other) = delete;
	BigBomb(BigBomb&& other) = delete;
	BigBomb& operator=(BigBomb&& other) = delete;


	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	bool canBeRemoved() const;

private:
	float m_Opacity;

	ParticleSystem* m_ParticleSystem;
	Timer* m_Timer;

	float m_Time;
};

