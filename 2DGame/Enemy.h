#pragma once
#include "Vector2f.h"
#include "Actor.h"


class Enemy : public Actor
{
public:
	Enemy(Rectf actorRect, Rectf window, ResourceManager* resourceManager);
	virtual ~Enemy();

	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;

	virtual void Update( float elapsedSec, const Level* level );
	virtual void Movement(const Actor* actor);

	virtual void Draw() const override;

	virtual void KillActor();

protected:
	float m_WalkSpeed;

	SpriteInfo* m_MovingSpriteInfo;
	Sprite* m_MovingSprite;

	SpriteInfo* m_DyingSpriteInfo;
	Sprite* m_DyingSprite;

	enum class ActionState { Idle, Moving, Dying };
	ActionState m_ActionState;

	virtual void Shoot(const Actor* actor);
private:

};

