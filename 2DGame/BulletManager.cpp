#include "pch.h"
#include "BulletManager.h"

#include "BlueBullets.h"
#include "YellowBullets.h"
#include "SingleBomb.h"
#include "BigBomb.h"
#include "JetBullets.h"

#include "Player.h"
#include "Actor.h"
#include "Enemy.h"

BulletManager::BulletManager(BulletType bulletType, ResourceManager* resourceManager)
	: m_BulletType{ bulletType }
	, m_HasFired{ false }
	, m_TimeElasped{}
	, m_ShootAmount{ 0 }
	, m_Wait{ false }
	, m_normalTime{ 0.8f }	
	, m_BulletCount{ 0 }
	, m_ResourceManager{ resourceManager }
{
	m_TimeTillFire = m_normalTime;
}

BulletManager::~BulletManager()
{
	for (Bullet* bullet : m_pBullets)
	{
		delete bullet;
		bullet = nullptr;
	}
	m_pBullets.clear();

	m_ResourceManager = nullptr;
}

void BulletManager::Shoot(Point2f spawnPoint, Vector2f direction, int amount, Point2f endPoint = Point2f{ 0, 0 } )
{
	if (m_HasFired == false)
	{
		m_ShootAmount = amount;
	}

	switch (m_BulletType)
	{
	case BulletManager::BulletType::blueBullets:
		if (m_HasFired == false || m_ShootAmount > 0 && m_Wait == false)
		{
			m_pBullets.push_back( new BlueBullets{ spawnPoint, direction, m_ResourceManager } );
			m_ResourceManager->Sounds()->PlaySound("Shooting");
			m_HasFired = true;
			m_Wait = true;
			m_TimeTillFire = m_normalTime / 10;
		}
		break;
	case BulletManager::BulletType::TankBullet:

		break;
	case BulletManager::BulletType::yellowBullets:
		if (m_HasFired == false)
		{
			m_pBullets.push_back(new YellowBullets{ spawnPoint, direction, m_ResourceManager });
			m_ResourceManager->Sounds()->PlaySound("Shooting");
			m_HasFired = true;
		}
		break;
	case BulletManager::BulletType::jetBullets:
		if (m_HasFired == false || m_ShootAmount > 0 && m_Wait == false)
		{
			m_pBullets.push_back(new JetBullets{ spawnPoint, direction, endPoint, m_ResourceManager });
			m_ResourceManager->Sounds()->PlaySound("Shooting");
			++m_BulletCount;
			if (m_BulletCount >= 2)
			{
				m_HasFired = true;
				m_Wait = true;
				m_BulletCount = 0;
			}
			m_TimeTillFire = m_normalTime / 10;
		}
		break;
	}
}

void BulletManager::Update(float elapsedSec)
{
	for (Bullet* bullet : m_pBullets)
	{
		bullet->Update(elapsedSec);
	}

	if (m_HasFired == true)
	{
		m_TimeElasped += elapsedSec;
		if (m_ShootAmount <= 0)
		{
			m_TimeTillFire = m_normalTime;
		}
		if (m_TimeElasped >= m_TimeTillFire)
		{
			if (m_ShootAmount <= 0)
			{
				m_HasFired = false;
				m_TimeElasped = 0;
			}
			else
			{
				m_Wait = false;
				m_TimeElasped = 0;
				--m_ShootAmount;
			}
		}
	}

	for (size_t index = 0; index < m_pBullets.size(); index++)
	{
		if (m_pBullets[index]->isReadyToDestroy())
		{
			m_pBullets[index] = m_pBullets.back();
			m_pBullets.pop_back();
		}
	}
}
void BulletManager::Draw() const
{
	for (Bullet* bullet : m_pBullets)
	{
		bullet->Draw();
	}
}

void BulletManager::CheckHit(Actor* actor)
{
	for (size_t index = 0; index < m_pBullets.size(); index++)
	{
		if (m_pBullets[index]->getBulletType() == Bullet::BulletType::singleBomb)
		{
			SingleBomb* pBomb{ static_cast<SingleBomb*>(m_pBullets[index]) };
			if (pBomb->canTouch())
			{
				if(utils::IsOverlapping(m_pBullets[index]->getRect(), actor->getActorRect()))
				{
					actor->KillActor();

					m_pBullets[index] = m_pBullets.back();
					m_pBullets.pop_back();
				}
			}
		}
		else if (m_pBullets[index]->getBulletType() == Bullet::BulletType::bigBomb)
		{
			BigBomb* pBigBomb{ static_cast<BigBomb*>(m_pBullets[index]) };
			if (utils::IsOverlapping(m_pBullets[index]->getRect(), actor->getActorRect()))
			{
				actor->KillActor();				
				if (pBigBomb->canBeRemoved())
				{
					m_pBullets[index] = m_pBullets.back();
					m_pBullets.pop_back();
				}				
			}
		}
		else if (m_pBullets[index]->getBulletType() == Bullet::BulletType::blueBullets)
		{
			if (utils::IsOverlapping(m_pBullets[index]->getRect(), actor->getActorRect()))
			{
				if (m_pBullets[index]->isDestroyin() == false)
				{
					actor->KillActor();
				}
				m_pBullets[index]->MakeReadyToDestroy();
			}
		}
		else if (utils::IsOverlapping(m_pBullets[index]->getRect(), actor->getActorRect()))
		{
			actor->KillActor();
			m_pBullets[index] = m_pBullets.back();
			m_pBullets.pop_back();
		}		
	}
}

BulletManager::BulletType BulletManager::getBulletType()
{
	return m_BulletType;
}
bool BulletManager::isDoneShoot()
{
	return !m_HasFired;
}

void BulletManager::AddBullet(BulletType bulletType, Point2f startPoint, Vector2f direction, Point2f endPoint)
{
	switch (bulletType)
	{
	case BulletManager::BulletType::singleBomb:
		m_pBullets.push_back(new SingleBomb{ startPoint, direction, endPoint, m_ResourceManager });
		break;
	case BulletManager::BulletType::bigBomb:
		m_pBullets.push_back(new BigBomb{ startPoint, direction, m_ResourceManager });
		m_ResourceManager->Sounds()->PlaySound("Explosion");
		break;
	}
}

