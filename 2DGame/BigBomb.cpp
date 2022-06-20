#include "pch.h"
#include "BigBomb.h"

BigBomb::BigBomb(Point2f startPoint, Vector2f direction, ResourceManager* resourceManager)
	: Bullet{ startPoint, resourceManager }
	, m_Opacity{ 1.f }
	, m_ParticleSystem{ new ParticleSystem{resourceManager} }
	, m_Timer{ new Timer{} }
	, m_Time{ 1.5f }
{
	m_BulletType = BulletType::bigBomb;

	m_BulletRect.width = 250;
	m_BulletRect.height = 100;
	m_BulletRect.left = startPoint.x - m_BulletRect.width / 2;
	m_BulletRect.bottom = startPoint.y;
	
	m_Timer->setTimeGoal(m_Time);
	m_ParticleSystem->AddParticles(m_Time, m_BulletRect, ParticleSystem::ParticleType::explosions, 10);
}

BigBomb::~BigBomb()
{
	delete m_ParticleSystem;
	m_ParticleSystem = nullptr;

	delete m_Timer;
	m_Timer = nullptr;
}

void BigBomb::Update(float elapsedSec)
{
	m_Timer->Update(elapsedSec);

	m_ParticleSystem->Update(elapsedSec);
	m_ParticleSystem->UpdateArea(m_BulletRect);
}

void BigBomb::Draw() const
{
	m_ParticleSystem->Draw();
}

bool BigBomb::canBeRemoved() const
{
	return m_Timer->isTimerDone();
}
