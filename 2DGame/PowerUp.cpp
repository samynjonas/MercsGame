#include "pch.h"
#include "PowerUp.h"
#include "Player.h"
#include "Sprite.h"

PowerUp::PowerUp(Rectf actorRect, Rectf window, PowerUpType powerUpType, ResourceManager* resourceManager)
	: Actor{ actorRect, window, 0, resourceManager }
	, m_PowerUpType{ powerUpType }
{
	m_ActorType = actorType::powerUp;
	InitializeTexture();	
}

PowerUp::~PowerUp()
{

}

void PowerUp::Update(float elapsedSec, const Level* level)
{
	m_IdleSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
}

void PowerUp::CheckPlayerColliding(Player* player)
{
	if (utils::IsOverlapping(m_ActorRect, player->getActorRect()))
	{
		m_ResourceManager->Sounds()->PlaySound("powerUp");

		switch (m_PowerUpType)
		{
		case PowerUp::PowerUpType::Power:
			player->IncreaseBombs(1);
			break;
		case PowerUp::PowerUpType::Food:
			player->IncreaseHealth(2);
			break;
		case PowerUp::PowerUpType::Health:
			player->IncreaseHealth(7);
			break;
		case PowerUp::PowerUpType::Bomb:
			player->IncreaseBombs(2);
			break;
		case PowerUp::PowerUpType::none:
			break;
		}
		m_IsActorDead = true;
	}
}

void PowerUp::InitializeTexture()
{
	switch (m_PowerUpType)
	{
	case PowerUp::PowerUpType::Power:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Power");
		break;
	case PowerUp::PowerUpType::Food:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Food");
		break;
	case PowerUp::PowerUpType::Health:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Health");
		break;
	case PowerUp::PowerUpType::Bomb:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Bomb");;
		break;
	}

	delete m_IdleSprite;
	m_IdleSprite = new Sprite{ m_IdleSpriteInfo->texture, m_IdleSpriteInfo->collums, m_IdleSpriteInfo->rows, m_IdleSpriteInfo->fps, m_IdleSpriteInfo->scale };

	m_ActorRect.width	= m_IdleSprite->getTextureRect(m_IdleSpriteInfo->sourceRect).width;
	m_ActorRect.height	= m_IdleSprite->getTextureRect(m_IdleSpriteInfo->sourceRect).height;
}