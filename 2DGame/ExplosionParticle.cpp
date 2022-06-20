#include "pch.h"
#include "ExplosionParticle.h"

ExplosionParticle::ExplosionParticle(Point2f pos, ResourceManager* resourceManager)
	: Particle{ pos }
	, m_SpriteInfo{ resourceManager->Sprites()->GetSprite("BigExplosion")}
	, m_ResourceManager{ resourceManager }
{
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };

	m_ParticleType = ParticleType::Explosions;
}

ExplosionParticle::~ExplosionParticle()
{
	m_ResourceManager = nullptr;
	m_SpriteInfo = nullptr;

	delete m_Sprite;
	m_Sprite = nullptr;
}

void ExplosionParticle::Update(float elapsedSec)
{
	m_Sprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
}

void ExplosionParticle::Draw() const
{
	m_Sprite->Draw(Rectf{ m_Position.x, m_Position.y, m_Sprite->getTextureRect(m_SpriteInfo->sourceRect).width, m_Sprite->getTextureRect(m_SpriteInfo->sourceRect).height }, m_SpriteInfo->sourceRect);
}

bool ExplosionParticle::removeParticle() const
{
	if (m_Sprite->HasDoneCycle())
	{
		return true;
	}
	return false;
}
