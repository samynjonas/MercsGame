#include "pch.h"
#include "JetBoss.h"

JetBoss::JetBoss(Rectf jetRect, Rectf Window, Rectf bossRoom, ResourceManager* resourceManager)
	: Actor{ jetRect, Window, m_FlySpeed, resourceManager }
	, m_MovePointIndex{ 0 }
	, m_ShootAmount{}
	, m_Health{ 100 }
	, m_ParticleSystem{ new ParticleSystem{resourceManager} }
	, m_Timer{ new Timer{} }
{
	m_IdleSpriteInfo = resourceManager->Sprites()->GetSprite("JetShooting");

	delete m_IdleSprite;
	m_IdleSprite = new Sprite{ m_IdleSpriteInfo->texture, m_IdleSpriteInfo->collums, m_IdleSpriteInfo->rows, m_IdleSpriteInfo->fps, m_IdleSpriteInfo->scale };

	delete m_pBullets;
	m_pBullets = new BulletManager{ BulletManager::BulletType::jetBullets, resourceManager };
	
	m_ActorType = actorType::boss;

	m_MovePoints.push_back(Point2f{ bossRoom.left + bossRoom.width / 3, jetRect.bottom });
	m_MovePoints.push_back(Point2f{ bossRoom.left + bossRoom.width / 3, jetRect.bottom + bossRoom.height / 8 });
	m_MovePoints.push_back(Point2f{ bossRoom.left + bossRoom.width / 8, jetRect.bottom });
	m_MovePoints.push_back(Point2f{ bossRoom.left + bossRoom.width / 8, jetRect.bottom + bossRoom.height / 8 });

	m_Timer->setTimeGoal(1.5f);
}

JetBoss::~JetBoss()
{
	delete m_ParticleSystem;
	m_ParticleSystem = nullptr;
	
	delete m_Timer;
	m_Timer = nullptr;
}

void JetBoss::Draw() const
{
	m_IdleSprite->Draw(getActorRect(), m_IdleSpriteInfo->sourceRect);
	if (m_CurrentActionState == actionState::dying)
	{
		m_ParticleSystem->Draw();
	}
	m_pBullets->Draw();

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_ActorRect);
	}
}

void JetBoss::Update(float elapsedSec, const Level* level)
{
	Point2f leftTurret{ m_ActorRect.left + m_ActorRect.width / 8.f, m_ActorRect.bottom + m_ActorRect.height / 3 };
	Point2f rightTurret{ m_ActorRect.left + m_ActorRect.width - m_ActorRect.width / 6.f, m_ActorRect.bottom + m_ActorRect.height / 3 };

	m_pBullets->Update(elapsedSec);

	if (m_CurrentActionState == actionState::moving)
	{
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::Cycle);
		m_CurrentActionState = actionState::Shooting;
	}
	else if (m_CurrentActionState == actionState::Shooting)
	{
		//Do shooting, if shooting is done
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
		if (m_IdleSprite->HasDoneCycle() )
		{
			++m_ShootAmount;
			m_IdleSprite->SwitchToFrame(0);
		}
		m_pBullets->Shoot( leftTurret,  Vector2f{ 0, -1 }, 3, Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom - 300 });
		m_pBullets->Shoot( rightTurret, Vector2f{ 0, -1 }, 3, Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom - 300 });

		//Movement
		movement(m_MovePointIndex);
		m_ActorRect.left   += m_Velocity.x * elapsedSec;
		m_ActorRect.bottom += m_Velocity.y * elapsedSec;

		if (m_ShootAmount >= 3)
		{
			m_MovePointIndex = rand() % m_MovePoints.size();
			m_CurrentActionState = actionState::moving;
			m_ShootAmount = 0;
		}
	}
	else if (m_CurrentActionState == actionState::dying)
	{
		m_Timer->Update(elapsedSec);
		m_ParticleSystem->Update(elapsedSec);
		m_ParticleSystem->UpdateArea(m_ActorRect);
		m_FlySpeed = 100;
		m_ActorRect.bottom -= m_FlySpeed * elapsedSec;

		if (m_Timer->isTimerDone())
		{
			m_IsActorDead = true;
		}
	}
	else if (m_CurrentActionState == actionState::starting)
	{
		m_Timer->Update(elapsedSec);
		if ( m_Timer->isTimerDone() )
		{
			m_CurrentActionState = actionState::moving;
		}
	}
}
void JetBoss::movement(int index)
{
	float offset{ 2 };
	bool isXGood{ false };

	if (m_ActorRect.left > m_MovePoints[index].x + offset)
	{
		//Move to left
		m_Velocity.x = m_FlySpeed * -1;
	}
	else if (m_ActorRect.left < m_MovePoints[index].x - offset)
	{
		//Move to right
		m_Velocity.x = m_FlySpeed;
	}
	else 
	{
		//Is in position
		m_Velocity.x = 0;
	}

	if (m_ActorRect.bottom > m_MovePoints[index].y + offset)
	{
		//Move down
		m_Velocity.y = m_FlySpeed * -1;
	}
	else if (m_ActorRect.bottom < m_MovePoints[index].y - offset)
	{
		//Move up
		m_Velocity.y = m_FlySpeed;
	}
	else
	{
		//Is in position
		m_Velocity.y = 0;
	}
}

void JetBoss::KillActor()
{
	if (m_Health > 0)
	{
		--m_Health;
	}
	else
	{
		if (m_CurrentActionState != actionState::dying)
		{
			m_ParticleSystem->AddParticles(5.f, m_ActorRect, ParticleSystem::ParticleType::explosions, 8);
			m_Timer->resetTimer();
			m_Timer->setTimeGoal(5);
		}		
		m_CurrentActionState = actionState::dying;
	}
}