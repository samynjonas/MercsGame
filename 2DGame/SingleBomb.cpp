#include "pch.h"
#include "SingleBomb.h"
#include "Sprite.h"

SingleBomb::SingleBomb(Point2f startPoint, Vector2f direction, Point2f endPoint, ResourceManager* resourceManager)
	: Bullet{ startPoint, resourceManager }
	, m_StartPoint{ startPoint }
	, m_endPoint{ endPoint }
	, explosionSize{ 30 }
	, m_CanTouch{ false }
	, m_Destroyed{ false }
	, m_Falling{ false }
	, m_LifeTime{ new Timer{} }
{
	m_BulletType = BulletType::singleBomb;
	m_SpriteInfo			= resourceManager->Sprites()->GetSprite("Bomb");
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };
	m_BulletHitSpriteInfo	= resourceManager->Sprites()->GetSprite("BigExplosion");
	m_BulletHitSprite = new Sprite{ m_BulletHitSpriteInfo->texture, m_BulletHitSpriteInfo->collums, m_BulletHitSpriteInfo->rows, m_BulletHitSpriteInfo->fps, m_BulletHitSpriteInfo->scale };

	m_Speed.x = m_Speed.y = 50;

	m_Speed.x *= direction.x;
	m_Speed.y *= direction.y;
	m_BulletRect.width = m_BulletRect.height = 10;

	m_LifeTime->setTimeGoal(2.5f);
}

SingleBomb::~SingleBomb()
{
	delete m_LifeTime;
	m_LifeTime = nullptr;
}

void SingleBomb::Update(float elapsedSec)
{
	m_Sprite->Update(elapsedSec, Sprite::CyleType::Step);
	m_LifeTime->Update(elapsedSec);

	if (m_CanTouch == false)
	{
		if (Distance(Point2f{ m_BulletRect.left, m_BulletRect.bottom }, m_endPoint) < m_Speed.x || m_LifeTime->isTimerDone())
		{
			//Start explosion
			m_BulletRect.width = m_BulletRect.height = 50;
			m_CanTouch = true;

			m_BulletHitSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
			m_ResourceManager->Sounds()->PlaySound("Explosion");
		}
		else
		{
			m_BulletRect.left += m_Speed.x * elapsedSec;
			m_BulletRect.bottom += m_Speed.y * elapsedSec;

			if (Distance(Point2f{ m_BulletRect.left, m_BulletRect.bottom }, m_endPoint) > Distance(m_StartPoint, m_endPoint) / 2)
			{
				m_BulletRect.width += 0.075f;
				m_BulletRect.height += 0.075f;
				if (m_Falling == true)
				{
					m_BulletRect.width = m_BulletRect.height = 50;
					m_CanTouch = true;

					m_BulletHitSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
					m_ResourceManager->Sounds()->PlaySound("Explosion");
				}
			}
			else
			{
				m_Falling = true;
				m_BulletRect.width -= 0.05f;
				m_BulletRect.height -= 0.05f;
				m_BulletHitSprite->SwitchToFrame(0);
			}
		}
	}
	else
	{
		m_BulletHitSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
		if (m_BulletHitSprite->HasDoneCycle())
		{
			m_Destroyed = true;
		}
	}	
}

void SingleBomb::Draw() const
{
	utils::SetColor(Color4f{ 0, 0, 1, 1 });
	//utils::FillRect(m_BulletRect);
	if (!m_Destroyed)
	{
		if (m_CanTouch)
		{
			//std::cout << "bomb exploded" << std::endl;
			m_BulletHitSprite->Draw(m_BulletRect, m_BulletHitSpriteInfo->sourceRect);
		}
		else
		{
			//std::cout << "bomb not exploded" << std::endl;
			m_Sprite->Draw(m_BulletRect, m_SpriteInfo->sourceRect);
		}
	}

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_BulletRect);
	}
}

float SingleBomb::Distance(Point2f p1, Point2f p2) const
{
	float distance;
	distance = sqrtf((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	return abs(distance);
}

bool SingleBomb::canTouch() const
{
	return m_CanTouch;
}
