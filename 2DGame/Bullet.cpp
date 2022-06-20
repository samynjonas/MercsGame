#include "pch.h"
#include "Bullet.h"
#include "Sprite.h"

Bullet::Bullet( Point2f startPoint, ResourceManager* resourceManager)
	: m_BulletRect{ Rectf{startPoint.x, startPoint.y, 10, 10 }}
	, m_Speed{ 500, 500 }
	, m_BulletType{ BulletType::none }
	, m_SpriteInfo{	resourceManager->Sprites()->GetSprite("SingleBullet") }
	, m_BulletHitSpriteInfo{ resourceManager->Sprites()->GetSprite("BulletHit") }
	, AwaitDestroy{ false }
	, Destroy{ false }
	, m_ResourceManager{ resourceManager }
{
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };
	m_BulletHitSprite = new Sprite{ m_BulletHitSpriteInfo->texture, m_BulletHitSpriteInfo->collums, m_BulletHitSpriteInfo->rows, m_BulletHitSpriteInfo->fps, m_BulletHitSpriteInfo->scale };

}

Bullet::~Bullet()
{
	m_ResourceManager = nullptr;
	m_SpriteInfo = nullptr;
	m_BulletHitSpriteInfo = nullptr;

	delete m_Sprite;
	m_Sprite = nullptr;

	delete m_BulletHitSprite;
	m_BulletHitSprite = nullptr;
}

void Bullet::Update(float elapsedSec)
{
	m_BulletRect.left   += m_Speed.x * elapsedSec;
	m_BulletRect.bottom += m_Speed.y * elapsedSec;
}

Point2f Bullet::getPosition() const
{
	return Point2f{ m_BulletRect.left, m_BulletRect.bottom };
}
Rectf Bullet::getRect() const
{
	return m_BulletRect;
}

Bullet::BulletType Bullet::getBulletType() const
{
	return m_BulletType;
}

bool Bullet::isReadyToDestroy() const
{
	return Destroy;
}
void Bullet::MakeReadyToDestroy()
{
	AwaitDestroy = true;
}
bool Bullet::isDestroyin() const
{
	return AwaitDestroy;
}
