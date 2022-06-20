#pragma once
#include "Actor.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "Timer.h"

#include <vector>

class SpriteManager;

class JetBoss final : public Actor
{
public:
	JetBoss(Rectf jetRect, Rectf Window, Rectf bossRoom, ResourceManager* resourceManager);
	virtual ~JetBoss();

	JetBoss(const JetBoss& other) = delete;
	JetBoss& operator=(const JetBoss& other) = delete;
	JetBoss(JetBoss&& other) = delete;
	JetBoss& operator=(JetBoss&& other) = delete;

	virtual void Update(float elapsedSec, const Level* level) override;
	virtual void Draw() const override;

	virtual void KillActor();

private:
	enum class actionState { moving, Shooting, dying, starting };
	actionState m_CurrentActionState{ actionState::starting };

	ParticleSystem* m_ParticleSystem;

	Timer* m_Timer;

	float m_Health;

	float m_FlySpeed{ 150 };

	std::vector<Point2f> m_MovePoints;
	int m_MovePointIndex;
	
	int m_ShootAmount;

	void movement(int index);
};

