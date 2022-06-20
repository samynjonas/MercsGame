#include "pch.h"
#include "Enemy.h"
#include "Level.h"
#include "Sprite.h"

Enemy::Enemy(Rectf actorRect, Rectf window, ResourceManager* resourceManager)
	: m_WalkSpeed{ 75 }
	, Actor{ actorRect, window, m_WalkSpeed, resourceManager }
	, m_ActionState{ ActionState::Idle }

{
	m_ActorType	= actorType::enemy;
	m_pBullets	= new BulletManager{ BulletManager::BulletType::yellowBullets, resourceManager };

	m_IdleSpriteInfo	= resourceManager->Sprites()->GetSprite("EnemyIdle");
	m_MovingSpriteInfo	= resourceManager->Sprites()->GetSprite("EnemyWalking");
	m_DyingSpriteInfo	= resourceManager->Sprites()->GetSprite("EnemyDying");

	m_IdleSprite	= new Sprite{ m_IdleSpriteInfo->texture, m_IdleSpriteInfo->collums, m_IdleSpriteInfo->rows, m_IdleSpriteInfo->fps, m_IdleSpriteInfo->scale };
	m_MovingSprite	= new Sprite{ m_MovingSpriteInfo->texture, m_MovingSpriteInfo->collums, m_MovingSpriteInfo->rows, m_MovingSpriteInfo->fps, m_MovingSpriteInfo->scale };
	m_DyingSprite	= new Sprite{ m_DyingSpriteInfo->texture, m_DyingSpriteInfo->collums, m_DyingSpriteInfo->rows, m_DyingSpriteInfo->fps, m_DyingSpriteInfo->scale };

}

Enemy::~Enemy()
{
	m_MovingSpriteInfo = nullptr;
	m_DyingSpriteInfo = nullptr;

	delete m_MovingSprite;
	m_MovingSprite = nullptr;

	delete m_DyingSprite;
	m_DyingSprite = nullptr;
}

void Enemy::Update(float elapsedSec, const Level* level)
{
	m_pBullets->Update(elapsedSec);

	if (m_ActionState == ActionState::Moving)
	{
		m_MovingSprite->Update(elapsedSec, Sprite::CyleType::Cycle);

		level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::horizontal, int(m_Velocity.x / abs(m_Velocity.x)));
		level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::vertical, int(m_Velocity.y / abs(m_Velocity.y)));

		m_ActorRect.left   += m_Velocity.x * elapsedSec;
		m_ActorRect.bottom += m_Velocity.y * elapsedSec;
	}
	else if (m_ActionState == ActionState::Idle)
	{
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::Step);
	}	
	else if (m_ActionState == ActionState::Dying)
	{
		m_DyingSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
		if (m_DyingSprite->HasDoneCycle())
		{
			m_IsActorDead = true;
		}
	}

	if (m_ActionState != ActionState::Dying)
	{	
		if (m_Velocity.x != 0 || m_Velocity.y != 0)
		{
			m_ActionState = ActionState::Moving;
		}
		else
		{
			m_ActionState = ActionState::Idle;
		}

		if (m_Velocity.x == 0)
		{
			//Vertical movement
			if (m_Velocity.y > 0)
			{
				//Going up
				//std::cout << "up" << std::endl;

				m_IdleSprite->SwitchToRow(6);
				m_MovingSprite->SwitchToRow(6);
			}
			else if (m_Velocity.y < 0)
			{
				//Going down
				//std::cout << "down" << std::endl;

				m_IdleSprite->SwitchToRow(2);
				m_MovingSprite->SwitchToRow(2);
			}
		}
		else if (m_Velocity.x > 0.1f)
		{
			//going right
			if (m_Velocity.y > 0.1f)
			{
				//right up
				//std::cout << "right-up" << std::endl;

				m_IdleSprite->SwitchToRow(-1);
				m_MovingSprite->SwitchToRow(-1);
			}
			else if (m_Velocity.y < 0.1f)
			{
				//right down
				//std::cout << "right-down" << std::endl;

				m_IdleSprite->SwitchToRow(1);
				m_MovingSprite->SwitchToRow(1);
			}
			else if (m_Velocity.y == 0.1f)
			{
				//straight right
				//std::cout << "right-straight" << std::endl;

				m_IdleSprite->SwitchToRow(0);
				m_MovingSprite->SwitchToRow(0);
			}
		}
		else if (m_Velocity.x < 0.1f)
		{
			//going left
			if (m_Velocity.y > 0)
			{
				//left up
				//std::cout << "left-up" << std::endl;

				m_IdleSprite->SwitchToRow(5);
				m_MovingSprite->SwitchToRow(5);
			}
			else if (m_Velocity.y < 0)
			{
				//left down
				//std::cout << "left-down" << std::endl;

				m_IdleSprite->SwitchToRow(3);
				m_MovingSprite->SwitchToRow(3);
			}
			else if (m_Velocity.y == 0)
			{
				//straight left
				//std::cout << "left-straight" << std::endl;

				m_IdleSprite->SwitchToRow(4);
				m_MovingSprite->SwitchToRow(4);
			}
		}			
	}
}

void Enemy::Draw() const
{
	m_pBullets->Draw();
	switch (m_ActionState)
	{
	case Enemy::ActionState::Idle:
		m_IdleSprite->Draw(m_ActorRect, m_IdleSpriteInfo->sourceRect);
		break;
	case Enemy::ActionState::Moving:
		m_MovingSprite->Draw(m_ActorRect, m_MovingSpriteInfo->sourceRect);
		break;
	case Enemy::ActionState::Dying:
		m_DyingSprite->Draw(m_ActorRect, m_DyingSpriteInfo->sourceRect);
		break;
	}

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_ActorRect);
	}
}
void Enemy::Movement( const Actor* actor )
{
	Shoot(actor);
	float offset{ 5.f };

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << m_ActorRect.left << " < " << actor->getActorRect().left << std::endl;
	}
	if (actor->getActorRect().left > m_ActorRect.left + offset)
	{
		m_Velocity.x = m_WalkSpeed;
	}
	else if (actor->getActorRect().left < m_ActorRect.left - offset)
	{
		m_Velocity.x = m_WalkSpeed * -1;
	}
	else
	{
		m_Velocity.x = 0;
	}

	if (actor->getActorRect().bottom > m_ActorRect.bottom + offset)
	{
		m_Velocity.y = m_WalkSpeed;
	}
	else if (actor->getActorRect().bottom < m_ActorRect.bottom - offset)
	{
		m_Velocity.y = m_WalkSpeed * -1;
	}
	else
	{
		m_Velocity.y = 0;
	}
}
void Enemy::KillActor()
{
	m_ActionState = ActionState::Dying;
}
void Enemy::Shoot(const Actor* actor)
{
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "PEW" << std::endl;
	}
}