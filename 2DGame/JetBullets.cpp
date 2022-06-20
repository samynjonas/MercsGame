#include "pch.h"
#include "JetBullets.h"
#include "Sprite.h"

JetBullets::JetBullets(Point2f startPoint, Vector2f direction, Point2f endPoint, ResourceManager* resourceManager)
	: Bullet{ startPoint, resourceManager }
	, m_EndPoint{ endPoint }
{
	m_Speed.x = 85.f;
	m_Speed.y *= direction.y;

	m_SpriteInfo = resourceManager->Sprites()->GetSprite("JetBullet");
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };
	m_BulletRect.width	= m_Sprite->getTextureRect(m_SpriteInfo->sourceRect).width;
	m_BulletRect.height = m_Sprite->getTextureRect(m_SpriteInfo->sourceRect).height;
}

JetBullets::~JetBullets()
{

}

void JetBullets::Draw() const
{
	m_Sprite->Draw(m_BulletRect, m_SpriteInfo->sourceRect);

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_BulletRect);
	}
}

void JetBullets::Update(float elapsedSec)
{
	movement(elapsedSec);
	m_BulletRect.bottom += m_Speed.y * elapsedSec;
}

void JetBullets::movement(float elapsedSec)
{
	if (m_BulletRect.left > m_EndPoint.x)
	{
		//move to left
		m_BulletRect.left += abs(m_Speed.x) * -1 * elapsedSec;
	}
	else if (m_BulletRect.left < m_EndPoint.x)
	{
		//move to right	
		m_BulletRect.left += abs(m_Speed.x) * elapsedSec;
	}

	if (m_BulletRect.bottom < m_EndPoint.y)
	{
		//Remove bullet
		MakeReadyToDestroy();
	}
}