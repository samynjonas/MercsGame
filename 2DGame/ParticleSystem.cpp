#include "pch.h"
#include "ParticleSystem.h"
#include "ExplosionParticle.h"

ParticleSystem::ParticleSystem(ResourceManager* resourceManager)
	: m_Area{ Rectf{} }
	, m_Timer{ new Timer{} }
	, m_NewParticleType{ ParticleType::none }
	, m_Delay{ new Timer{} }
	, m_ResourceManager{ resourceManager }
{
	m_Delay->setTimeGoal(0.125f);
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* m_pParticle : m_Particles)
	{
		delete m_pParticle;
		m_pParticle = nullptr;
	}
	m_Particles.clear();

	delete m_Timer;
	delete m_Delay;

	m_Timer = nullptr;
	m_Delay = nullptr;

	m_ResourceManager = nullptr;
}

void ParticleSystem::Update(float elapsedSec)
{
	m_Delay->Update(elapsedSec);

	if (m_Timer->isTimerDone() == false)
	{
		m_Timer->Update(elapsedSec);
		if (m_Delay->isTimerDone())
		{
			pushParticle();
			m_Delay->resetTimer();
		}
	}

	for (SIZE_T index = 0; index < m_Particles.size(); index++)
	{
		m_Particles[index]->Update(elapsedSec);
		if (m_Particles[index]->removeParticle())
		{
			m_Particles[index] = m_Particles.back();
			m_Particles.pop_back();
		}
	}
}

void ParticleSystem::Draw() const
{
	for (Particle* m_pParticle : m_Particles)
	{
		m_pParticle->Draw();
	}
}

void ParticleSystem::AddParticles(float duration, Rectf area, ParticleType particleType, float density)
{
	if (density > 10) density = 10;
	m_Delay->setTimeGoal( 1.f / density );

	m_NewParticleType = particleType;
	m_Area = area;
	m_Timer->setTimeGoal(duration);
}

void ParticleSystem::pushParticle()
{
	switch (m_NewParticleType)
	{
	case ParticleType::blackNormal:
		m_Particles.push_back(new Particle{ RandomPointInArea() });
		break;
	case ParticleType::explosions:
		m_Particles.push_back(new ExplosionParticle{ RandomPointInArea(), m_ResourceManager });
		break;
	}
}

Point2f ParticleSystem::RandomPointInArea()
{
	Point2f point;
	point.x = float(rand() % int(m_Area.width));
	point.y = float(rand() % int(m_Area.height));

	point.x += m_Area.left;
	point.y += m_Area.bottom;
	
	return point;
}

void ParticleSystem::UpdateArea(Rectf area)
{
	m_Area = area;
}