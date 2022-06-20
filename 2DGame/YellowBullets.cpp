#include "pch.h"
#include "YellowBullets.h"
#include "Sprite.h"

YellowBullets::YellowBullets(Point2f startPoint, Vector2f direction, ResourceManager* resourceManager)
	: Bullet{ startPoint, resourceManager }
{
	m_Speed.x *= direction.x;
	m_Speed.y *= direction.y;
	m_BulletRect.width = m_BulletRect.height = 10;

	m_SpriteInfo = resourceManager->Sprites()->GetSprite("SingleBullet");
	m_Sprite = new Sprite{ m_SpriteInfo->texture, m_SpriteInfo->collums, m_SpriteInfo->rows, m_SpriteInfo->fps, m_SpriteInfo->scale };

}

YellowBullets::~YellowBullets()
{

}

void YellowBullets::Draw() const
{
	utils::SetColor(Color4f{ 0.95f, 0.95f, 0, 1 });
	m_Sprite->Draw(m_BulletRect, m_SpriteInfo->sourceRect);

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_BulletRect);
	}
}