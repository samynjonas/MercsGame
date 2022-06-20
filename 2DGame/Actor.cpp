#include "pch.h"
#include "Actor.h"
#include "utils.h"
#include "Sprite.h"

#include "BulletManager.h"

Actor::Actor(Rectf actorRect, Rectf window, float walkSpeed, ResourceManager* resourceManager)
	: m_Window{ window }
	, m_ActorType{}
	, m_IsActorDead{ false }
	, m_ResourceManager{ resourceManager }
	, m_IdleSprite{ nullptr }
	, m_IdleSpriteInfo{ nullptr }
{	
	m_Velocity = Vector2f{ walkSpeed, walkSpeed };
	m_ActorRect = actorRect;
	
	m_pBullets = new BulletManager{ BulletManager::BulletType::none, resourceManager };
}

Actor::~Actor()
{
	delete m_pBullets;
	m_pBullets = nullptr;

	m_IdleSprite = nullptr;
	delete m_IdleSprite;

	m_ResourceManager = nullptr;
	m_IdleSpriteInfo = nullptr;
}

void Actor::Draw() const
{
	m_IdleSprite->Draw(getActorRect(), m_IdleSpriteInfo->sourceRect);
	m_pBullets->Draw();

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_ActorRect);
	}
}

void Actor::Update(float elapsedSec, const Level* level)
{
	//Move the actor
	m_ActorRect.left   += m_Velocity.x * elapsedSec;
	m_ActorRect.bottom += m_Velocity.y * elapsedSec;

	//Level collision
	level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::horizontal, int(m_Velocity.x / abs(m_Velocity.x)));
	level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::vertical,   int(m_Velocity.y / abs(m_Velocity.y)));

	//Bullets updating
	m_pBullets->Update(elapsedSec);
}

Rectf Actor::getActorRect() const
{
	return m_ActorRect;
}
void Actor::BulletHitActor(Actor* actor)
{
	m_pBullets->CheckHit(actor);
}
Actor::actorType Actor::getActorType() const
{
	return m_ActorType;
}
void Actor::KillActor()
{
	//m_ActorRect.height /= 2.f;
}
bool Actor::IsActorDead() const
{
	return m_IsActorDead;
}

void Actor::SetPosition(Point2f pos)
{
	m_ActorRect.left = pos.x;
	m_ActorRect.bottom = pos.y;
}
