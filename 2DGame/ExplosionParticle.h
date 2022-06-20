#pragma once
#include "Particle.h"
#include "Sprite.h"

#include "SpriteManager.h"
#include "ResourceManager.h"

class ExplosionParticle final: public Particle
{
public:
	ExplosionParticle(Point2f pos, ResourceManager* resourceManager);
	~ExplosionParticle();

	ExplosionParticle(const ExplosionParticle& other) = delete;
	ExplosionParticle& operator=(const ExplosionParticle& other) = delete;
	ExplosionParticle(ExplosionParticle&& other) = delete;
	ExplosionParticle& operator=(ExplosionParticle&& other) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	virtual bool removeParticle() const override;

private:
	SpriteInfo* m_SpriteInfo;
	Sprite* m_Sprite;
	ResourceManager* m_ResourceManager;

};

