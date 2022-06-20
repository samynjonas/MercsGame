#include "pch.h"
#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(Point2f pos, float spawnDelay, int maxSpawned, float range, float despawnRange, Rectf window, ResourceManager* resourceManager)
	: m_Position{ pos }
	, m_SpawnDelay{ spawnDelay }
	, m_MaxSpawned{ maxSpawned }
	, m_CurrentAmount{ 0 }
	, m_Range{ range }
	, m_SpawnTimer{ new Timer{} }
	, m_IsInRange{ false }
	, m_ResourceManager{ resourceManager }
	, m_Window{ window }
	, m_DespawnRange{ despawnRange }
	, m_Score{ 0 }
	, m_KillScore{ 50 }
{
	m_SpawnTimer->setTimeGoal(spawnDelay);
}

EnemySpawner::EnemySpawner(const Actor* enemy, float spawnDelay, int maxSpawned, float range, float despawnRange, Rectf window, ResourceManager* resourceManager)
	: m_Position{ Point2f{enemy->getActorRect().left, enemy->getActorRect().bottom }}
	, m_SpawnDelay{ spawnDelay }
	, m_MaxSpawned{ maxSpawned }
	, m_CurrentAmount{ 0 }
	, m_Range{ range }
	, m_SpawnTimer{ new Timer{} }
	, m_IsInRange{ false }
	, m_ResourceManager{ resourceManager }
	, m_Window{ window }
	, m_DespawnRange{ despawnRange }
	, m_Score{ 0 }
	, m_KillScore{ 50 }
{
	m_SpawnTimer->setTimeGoal(spawnDelay);
}

EnemySpawner::~EnemySpawner()
{
	delete m_SpawnTimer;
	m_SpawnTimer = nullptr;

	m_ResourceManager = nullptr;
}

void EnemySpawner::isInRange(Point2f point)
{
	if (point.x > m_Position.x - m_Range && point.x < m_Position.x + m_Range)
	{
		if (point.y > m_Position.y - m_Range && point.y < m_Position.y + m_Range)
		{
			m_IsInRange = true;
		}
		else
		{
			m_IsInRange = false;
		}
	}
	else if (abs(getLength(point, m_Position)) > m_DespawnRange)
	{
		m_IsInRange = false;
		if (m_CurrentAmount > 0)
		{
			killAll();
		}
	}
	else
	{
		m_IsInRange;
	}
}

void EnemySpawner::Update(float elapsedSec)
{
	m_SpawnTimer->Update(elapsedSec);
	m_CurrentAmount = int(m_Enemies.size());

	if (m_IsInRange)
	{
		if (m_CurrentAmount < m_MaxSpawned)
		{
			if (m_SpawnTimer->isTimerDone())
			{
				SpawnEnemy();
				m_SpawnTimer->resetTimer();
			}
		}
	}

	CheckForDeath();
}

void EnemySpawner::EnemyUpdating(Player* player, float elapsedSec, const Level* level)
{
	for (size_t index = 0; index < m_Enemies.size(); index++)
	{
		m_Enemies[index]->Update(elapsedSec, level);
		m_Enemies[index]->Movement(player);
		m_Enemies[index]->BulletHitActor(player);

		player->CollidingWithEnemy(m_Enemies[index]);
		player->BulletHitActor(m_Enemies[index]);

		if (player->getActorRect().bottom > m_Enemies[index]->getActorRect().bottom + 300)
		{
			m_Enemies[index]->KillActor();
		}
	}
}

void EnemySpawner::Draw() const
{
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 0, 1, 1, 0.3f });
		utils::FillEllipse(m_Position, m_Range, m_Range);

		utils::SetColor(Color4f{ 1, 1, 0, 0.8f });
		utils::FillEllipse(m_Position, 10, 10);
	}

	for (size_t index = 0; index < m_Enemies.size(); index++)
	{
		m_Enemies[index]->Draw();
	}
}

void EnemySpawner::SpawnEnemy()
{
	float randomX{ float(rand() % 10 - 5) };
	float randomY{ float(rand() % 10 - 5) };

	m_Enemies.push_back(new EnemyAssaultTrooper{ Rectf{ m_Position.x + randomX, m_Position.y + randomY, 30, 40}, m_Window, m_ResourceManager });
}
void EnemySpawner::killAll()
{
	for (size_t index = 0; index < m_Enemies.size(); index++)
	{
		m_Enemies[index]->KillActor();
	}
}

void EnemySpawner::CheckForDeath()
{
	for (size_t index = 0; index < m_Enemies.size(); index++)
	{
		if (m_Enemies[index]->IsActorDead())
		{
			m_Enemies[index] = m_Enemies.back();
			m_Enemies.pop_back();
			m_Score += m_KillScore;
		}
	}
}

float EnemySpawner::getLength(Point2f p1, Point2f p2)
{
	float length{};
	length = sqrt((p1.x + p2.x) * (p1.x + p2.x) + (p1.y + p2.y) * (p1.y + p2.y));
	return length;
}

std::vector<EnemyAssaultTrooper*>& EnemySpawner::GetEnemies()
{
	return m_Enemies;
}

int EnemySpawner::getScore()
{
	int temp{ m_Score };
	m_Score = 0;
	return temp;
}