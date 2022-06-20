#pragma once
#include "Enemy.h"
class EnemyAssaultTrooper final : public Enemy
{
public:
	EnemyAssaultTrooper(Rectf actorRect, Rectf window, ResourceManager* resourceManager);
	virtual ~EnemyAssaultTrooper();

	EnemyAssaultTrooper(const EnemyAssaultTrooper& other) = delete;
	EnemyAssaultTrooper& operator=(const EnemyAssaultTrooper& other) = delete;
	EnemyAssaultTrooper(EnemyAssaultTrooper&& other) = delete;
	EnemyAssaultTrooper& operator=(EnemyAssaultTrooper&& other) = delete;

	virtual void Movement(const Actor* actor) override;

protected:
	virtual void Shoot(const Actor* actor);

private:
	enum class AttackType{ Bombing, Shooting, Melee, Nothing };
	AttackType m_CurrentAttack;

	bool m_InRange;
	bool m_CanBombAgain;

	Point2f m_wanderingPoint;
	bool m_setWander;

	float Distance(const Actor* actor);
	void MoveTo(float range, const Actor* actor);
	void AssaultShoot(const Actor* actor);
	void ThrowBomb(const Actor* actor);

	Vector2f getDirection(const Actor* actor) const;
};