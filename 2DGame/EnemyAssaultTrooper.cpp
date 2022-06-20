#include "pch.h"
#include "EnemyAssaultTrooper.h"
#include "Actor.h"

EnemyAssaultTrooper::EnemyAssaultTrooper(Rectf actorRect, Rectf window, ResourceManager* resourceManager)
	: Enemy{actorRect, window, resourceManager }
	, m_CurrentAttack{ AttackType::Nothing }
	, m_InRange{ false }
	, m_CanBombAgain{ true }
	, m_setWander{ false }
{
	m_wanderingPoint = Point2f{ m_ActorRect.left, m_ActorRect.bottom };
}

EnemyAssaultTrooper::~EnemyAssaultTrooper()
{

}

void EnemyAssaultTrooper::Movement(const Actor* actor)
{
	float bombRange{ 200 }, ShootRange{ 150 }, MeleeRange{ 0 };

	switch (m_CurrentAttack)
	{
	case EnemyAssaultTrooper::AttackType::Bombing:
		//std::cout << "Move to bomb range" << std::endl;
		MoveTo(bombRange, actor);
		break;
	case EnemyAssaultTrooper::AttackType::Shooting:
		//std::cout << "Move to Shoot range" << std::endl;
		MoveTo(ShootRange, actor);

		break;
	case EnemyAssaultTrooper::AttackType::Melee:
		//std::cout << "Move to Melee range" << std::endl;
		MoveTo(MeleeRange, actor);

		break;
	case EnemyAssaultTrooper::AttackType::Nothing:
		//std::cout << "Wander around" << std::endl;
		MoveTo(bombRange, actor);
		break;
	}

	Shoot(actor);
}

float EnemyAssaultTrooper::Distance(const Actor* actor)
{
	Point2f playerPos{ actor->getActorRect().left + actor->getActorRect().width / 2, actor->getActorRect().bottom + actor->getActorRect().height / 2 };
	Point2f myPos{ m_ActorRect.left + m_ActorRect.width / 2,  m_ActorRect.bottom + m_ActorRect.height / 2 };

	float distance{ sqrtf(((playerPos.x - myPos.x) * (playerPos.x - myPos.x)) + ((playerPos.y - myPos.y) * (playerPos.y - myPos.y))) };
	return distance;
}
void EnemyAssaultTrooper::MoveTo( float range, const Actor* actor)
{
	Point2f playerPos{ actor->getActorRect().left + actor->getActorRect().width / 2, actor->getActorRect().bottom + actor->getActorRect().height / 2 };
	Point2f myPos{ m_ActorRect.left + m_ActorRect.width / 2,  m_ActorRect.bottom + m_ActorRect.height / 2 };

	float offset{ 15.f };

	//Move to position
	if (Distance(actor) > range + 2.f)
	{
		//Move closer
		if (myPos.x > playerPos.x + 5.f)
		{
			//Player is on the right
			m_Velocity.x = m_WalkSpeed * -1;
		}
		else if(myPos.x < playerPos.x - 5.f)
		{
			//Player is on the left
			m_Velocity.x = m_WalkSpeed;
		}
		else
		{
			m_Velocity.x = 0;
		}

		if (myPos.y > playerPos.y + 5.f)
		{
			//Player is under enemy
			m_Velocity.y = m_WalkSpeed * -1;
		}
		else if(myPos.y < playerPos.y - 5.f)
		{
			m_Velocity.y = m_WalkSpeed;
		}
		else
		{
			m_Velocity.y = 0;
		}
	}
	else if (Distance(actor) < range - 2.f)
	{

		//Go further
		if (myPos.x > playerPos.x)
		{
			//Player is on the right
			m_Velocity.x = m_WalkSpeed;
		}
		else
		{
			//Player is on the left
			m_Velocity.x = m_WalkSpeed * -1;
		}

		if (myPos.y > playerPos.y)
		{
			//Player is under enemy
			m_Velocity.y = m_WalkSpeed;
		}
		else
		{
			m_Velocity.y = m_WalkSpeed * -1;
		}
	}
	else
	{
		m_Velocity.x = 0.f;
		m_Velocity.y = 0.f;
	}

	//Check is in range
	if (Distance(actor) > range - offset && Distance(actor) < range + offset)
	{
		m_InRange = true;
	}
	else
	{
		m_InRange = false;
	}
}

void EnemyAssaultTrooper::Shoot(const Actor* actor)
{
	if (m_InRange)
	{
		switch (m_CurrentAttack)
		{
		case EnemyAssaultTrooper::AttackType::Bombing:
			ThrowBomb(actor);
			m_CurrentAttack = AttackType::Shooting;
			m_CanBombAgain = false;
			break;
		case EnemyAssaultTrooper::AttackType::Shooting:
			if ( m_pBullets->isDoneShoot() )
			{
				AssaultShoot(actor);
				if (m_CanBombAgain == true)
				{
					m_CurrentAttack = AttackType::Bombing;
				}
				else
				{
					//Shoot again or melee -------------------------- Needs better reason
					if (rand() % 10 != 1)
					{
						m_CurrentAttack = AttackType::Shooting;
					}
					else
					{
						m_CurrentAttack = AttackType::Melee;
					}
				}
			}
			break;
		case EnemyAssaultTrooper::AttackType::Melee:
			if (m_CanBombAgain == true)
			{
				m_CurrentAttack = AttackType::Bombing;
			}
			else
			{
				m_CurrentAttack = AttackType::Shooting;
			}
			break;
		case EnemyAssaultTrooper::AttackType::Nothing:
			m_CurrentAttack = AttackType::Bombing;
			break;
		}
	}
}

void EnemyAssaultTrooper::AssaultShoot(const Actor* actor)
{	
	m_pBullets->Shoot(Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 }, getDirection(actor), 1, Point2f{ 0, 0 });
}
void EnemyAssaultTrooper::ThrowBomb(const Actor* actor)
{
	m_pBullets->AddBullet(BulletManager::BulletType::singleBomb, Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 }, getDirection(actor), Point2f{actor->getActorRect().left + actor->getActorRect().width / 2, actor->getActorRect().bottom + actor->getActorRect().height / 2 });
}

Vector2f EnemyAssaultTrooper::getDirection(const Actor* actor) const
{
	float offset{ actor->getActorRect().height };
	float horizontalDirection{};
	if (m_ActorRect.left > actor->getActorRect().left + offset)
	{
		horizontalDirection = -1;
	}
	else if (m_ActorRect.left < actor->getActorRect().left - offset)
	{
		horizontalDirection = 1;
	}
	else
	{
		horizontalDirection = 0;
	}

	float verticalDirection{};
	if (m_ActorRect.bottom > actor->getActorRect().bottom + offset)
	{
		verticalDirection = -1;
	}
	else if (m_ActorRect.bottom < actor->getActorRect().bottom - offset)
	{
		verticalDirection = 1;
	}
	else
	{
		verticalDirection = 0;
	}

	return Vector2f{ horizontalDirection, verticalDirection };
}

