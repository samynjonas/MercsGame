#pragma once
#include "Sprite.h"
#include "Level.h"
#include "Camera.h"
#include "Timer.h"

#include "BulletManager.h"

#include "SpriteManager.h"

#include "Actor.h"
#include "Enemy.h"

class Object;

class Player final: public Actor
{
public:
	Player(Rectf playerRect, Rectf window, ResourceManager* resourceManager);
	virtual ~Player();

	Player(const Player& other) = delete;
	Player& operator=(const Player& other) = delete;
	Player(Player&& other) = delete;
	Player& operator=(Player&& other) = delete;

	virtual void Update(float elapsedSec, const Level* level) override;
	virtual void Draw() const override;

	void ObjectColliding(const Object* object);
	void CollidingWithEnemy(const Enemy* enemy);

	void getInput(int horizontal, int vertical, const Level* level);
	void CheckCameraCollision( const Camera* camera );

	void IncreaseHealth(int amount);
	void IncreaseBombs(int amount);

	virtual void KillActor();

	int getMaxHealth() const;
	int getHealth() const;
	int getBombs() const;

private:
	enum class ActionState { idle, moving, dying };

	Timer* m_Timer;
	Timer* m_Delay;

	ResourceManager* m_ResourceManager;

	SpriteInfo* m_WalkingSpriteInfo;
	Sprite* m_WalkingSprite;
	SpriteInfo* m_DyingSpriteInfo;
	Sprite* m_DyingSprite;

	Vector2f m_Direction{1, 0};
	float m_WalkSpeed{ 150 };

	float m_MaxHealth;
	float m_Health;

	int m_Bombs;

	bool m_IsHit;
	bool m_DrawHit;
	float m_Opacity;

	ActionState m_ActionState;

	bool ThrowingBomb;

	void CalculateSpeed();
	void HorizontalColliding(const Object* object);
	void VerticalColliding(const Object* object);

};

