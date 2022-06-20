#pragma once
#include "Sprite.h"
#include "Timer.h"

class Particle
{
public:
	enum class ParticleType {BlackParticle, Explosions};
	Particle(Point2f pos);
	virtual ~Particle();

	Particle(const Particle& other) = delete;
	Particle& operator=(const Particle& other) = delete;
	Particle(Particle&& other) = delete;
	Particle& operator=(Particle&& other) = delete;

	virtual void Update(float elapsedSec);
	virtual void Draw() const;

	virtual bool removeParticle() const;

protected:
	ParticleType m_ParticleType;
	Point2f m_Position;

	float m_Opacity{};

};

