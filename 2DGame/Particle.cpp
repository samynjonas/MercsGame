#include "pch.h"
#include "Particle.h"

Particle::Particle(Point2f pos)
	: m_ParticleType{ ParticleType::BlackParticle }
	, m_Opacity{ 255 }
	, m_Position{ pos }
{


}

Particle::~Particle()
{
	
}

void Particle::Update(float elapsedSec)
{
	--m_Opacity;
}

void Particle::Draw() const
{
	utils::SetColor(Color4f{ 1, 0, 0, m_Opacity / 255.f });
	utils::FillRect(Rectf{ m_Position.x, m_Position.y, 10, 10 });
}

bool Particle::removeParticle() const
{
	if (m_Opacity <= 0)
	{
		return true;
	}
	return false;
}


