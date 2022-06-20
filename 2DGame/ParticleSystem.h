#pragma once
#include "Particle.h"

#include "Timer.h"
#include <vector>
#include "SpriteManager.h"
#include "ResourceManager.h"

class ParticleSystem final
{
public:
	enum class ParticleType { blackNormal, explosions, none };
	ParticleSystem(ResourceManager* resourceManager);
	~ParticleSystem();

	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void AddParticles(float duration, Rectf area, ParticleType particleType, float density);
	void UpdateArea( Rectf area );

private:
	std::vector<Particle*> m_Particles;

	ResourceManager* m_ResourceManager;

	ParticleType m_NewParticleType;
	Rectf m_Area;
	Timer* m_Timer;
	Timer* m_Delay;

	void pushParticle();
	Point2f RandomPointInArea();
};

