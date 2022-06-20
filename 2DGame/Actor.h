#pragma once
#include "Level.h"
#include "BulletManager.h"
#include "SpriteManager.h"

#include "ResourceManager.h"

class BulletManager;
class Sprite;

class Actor
{
public:
	enum class actorType{ player, enemy, actor, object, powerUp, boss };

	Actor(Rectf actorRect, Rectf window, float walkSpeed, ResourceManager* resourceManager);
	virtual ~Actor();

	Actor(const Actor& other) = delete;
	Actor& operator=(const Actor& other) = delete;
	Actor(Actor&& other) = delete;
	Actor& operator=(Actor&& other) = delete;

	virtual void Draw() const;
	virtual void Update(float elapedSec, const Level* level);

	void BulletHitActor(Actor* actor);
	
	virtual void KillActor();

	Rectf getActorRect() const;
	actorType getActorType() const;
	bool IsActorDead() const;

	void SetPosition(Point2f pos);

protected:
	Rectf m_Window;
	
	Rectf m_ActorRect;
	Vector2f m_Velocity;
	actorType m_ActorType;

	BulletManager* m_pBullets;
	bool m_IsActorDead;

	ResourceManager* m_ResourceManager;
	Sprite* m_IdleSprite;
	SpriteInfo* m_IdleSpriteInfo;

private:

};

