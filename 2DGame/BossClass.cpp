#include "pch.h"
#include "BossClass.h"
#include "JetBoss.h"

BossClass::BossClass(Rectf bossRoom, BossType boss, Rectf window, ResourceManager* resourceManager)
	: m_BossRoom{ bossRoom }
	, m_CurrentBoss{ boss }
	, m_SpawnBoss{ false }
	, m_Boss{ nullptr }
	, m_ResourceManager{ resourceManager }
{
	switch (m_CurrentBoss)
	{
	case BossClass::BossType::Jet:
		//Actor = new Jett{};
		m_Boss = new JetBoss{ Rectf{ bossRoom.left + bossRoom.width / 2, bossRoom.bottom + bossRoom.height - bossRoom.height / 5, 300, 200 }, window, m_BossRoom, resourceManager };
		break;
	case BossClass::BossType::Tank:
		//Actor = new Tank{};
		break;
	}
}

BossClass::~BossClass()
{
	delete m_Boss;
	m_Boss = nullptr;

	m_ResourceManager = nullptr;
}

void BossClass::Update(float elapsedSec, const Level* level, Player* playerOne, Player* playerTwo = nullptr)
{
	if (m_SpawnBoss == true)
	{
		m_Boss->Update(elapsedSec, level);

		playerOne->BulletHitActor(m_Boss);
		m_Boss->BulletHitActor(playerOne);

		if (playerTwo != nullptr)
		{
			playerTwo->BulletHitActor(m_Boss);
			m_Boss->BulletHitActor(playerTwo);
		}
	}
	else
	{
		isActorInRoom(playerOne);
		if (playerTwo != nullptr)
		{
			isActorInRoom(playerTwo);
		}
	}	
}

void BossClass::Draw() const
{
	if (m_SpawnBoss == true)
	{
		m_Boss->Draw();
	}

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_BossRoom);
	}
}


Point2f BossClass::getMiddlePoint() const
{
	return Point2f{ m_BossRoom.left + m_BossRoom.width / 2, m_BossRoom.bottom + ((m_BossRoom.height / 4) * 3) };
}

bool BossClass::isActorInRoom(const Player* player)
{
	if (player->getActorRect().left > m_BossRoom.left + m_BossRoom.width / 2 && player->getActorRect().bottom > m_BossRoom.bottom)
	{
		m_SpawnBoss = true;
		return true;
	}
	return false;
}

bool BossClass::isBossActive() const
{
	return m_SpawnBoss;
}

BossClass::BossType BossClass::getBossType() const
{
	return m_CurrentBoss;
}

bool BossClass::IsActorDead() const
{
	return m_Boss->IsActorDead();
}