#include "pch.h"
#include "BlueBullets.h"
#include "Sprite.h"

BlueBullets::BlueBullets(Point2f startPoint, Vector2f direction, ResourceManager* resourceManager)
	: Bullet{ startPoint, resourceManager }
{
	m_BulletType = BulletType::blueBullets;

	m_Speed.x *= direction.x;
	m_Speed.y *= direction.y;

	m_BulletRect.width = m_BulletRect.height = 10;

	m_SpriteInfo = resourceManager->Sprites()->GetSprite("SingleBullet");
	m_BulletHitSpriteInfo = resourceManager->Sprites()->GetSprite("BulletHit");
	
	delete m_Sprite;
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };
	
	delete m_BulletHitSprite;
	m_BulletHitSprite = new Sprite{ m_BulletHitSpriteInfo->texture, m_BulletHitSpriteInfo->collums, m_BulletHitSpriteInfo->rows, m_BulletHitSpriteInfo->fps, m_BulletHitSpriteInfo->scale };

	m_Sprite->SwitchToRow(1);

	if (direction.x == 0)
	{
		//Vertical movement
		if (direction.y > 0)
		{
			//Going up
			m_Sprite->SwitchToFrame(0);
		}
		else if (direction.y < 0)
		{
			//Going down
			m_Sprite->SwitchToFrame(0, false, true);
		}
	}
	else if (direction.x > 0)
	{
		//going right
		if (direction.y > 0)
		{
			//right up
			m_Sprite->SwitchToFrame(1);
		}
		else if (direction.y < 0)
		{
			//right down
			m_Sprite->SwitchToFrame(1, false, true);
		}
		else if (direction.y == 0)
		{
			//straight right
			m_Sprite->SwitchToFrame(2);
		}
	}
	else if (direction.x < 0)
	{
		//going left
		if (direction.y > 0)
		{
			//left up
			m_Sprite->SwitchToFrame(1, true);
		}
		else if (direction.y < 0)
		{
			//left down
			m_Sprite->SwitchToFrame(1, true, true);
		}
		else if (direction.y == 0)
		{
			//straight left
			m_Sprite->SwitchToFrame(2, true);
		}
	}

}

BlueBullets::~BlueBullets()
{

}

void BlueBullets::Update(float elapsedSec)
{
	m_Sprite->Update(elapsedSec, Sprite::CyleType::Step);

	if (AwaitDestroy && m_BulletHitSprite->HasDoneCycle())
	{
		Destroy = true;
	}

	if (!AwaitDestroy)
	{
		m_BulletRect.left   += m_Speed.x * elapsedSec;
		m_BulletRect.bottom += m_Speed.y * elapsedSec;
	}
	else
	{
		//std::cout << "hit animation" << std::endl;
		m_BulletHitSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
	}
}

void BlueBullets::Draw() const
{
	utils::SetColor(Color4f{ 0, 0, 1, 1 });
	//utils::FillRect(m_BulletRect);

	if (AwaitDestroy)
	{
		//std::cout << "animation" << std::endl;
		m_BulletHitSprite->Draw(m_BulletRect, m_BulletHitSpriteInfo->sourceRect);
	}
	else
	{
		m_Sprite->Draw(m_BulletRect, m_SpriteInfo->sourceRect);
	}

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_BulletRect);
	}
}
