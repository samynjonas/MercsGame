#pragma once
#include <vector>
#include "Timer.h"

#include "EnemyAssaultTrooper.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Actor.h"

class EnemySpawner final
{
public:
	EnemySpawner(Point2f pos, float spawnDelay, int maxSpawned, float range, float despawnRange, Rectf window, ResourceManager* resourceManager);
	EnemySpawner(const Actor* enemy, float spawnDelay, int maxSpawned, float range, float despawnRange, Rectf window, ResourceManager* resourceManager);
	~EnemySpawner();

	EnemySpawner(const EnemySpawner& other) = delete;
	EnemySpawner& operator=(const EnemySpawner& other) = delete;
	EnemySpawner(EnemySpawner&& other) = delete;
	EnemySpawner& operator=(EnemySpawner&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void isInRange(Point2f point);
	void EnemyUpdating(Player* player, float elapsedSec, const Level* level);

	void killAll();
	float getLength(Point2f p1, Point2f p2);
	int getScore();
	

	std::vector<EnemyAssaultTrooper*>& GetEnemies();

private:
	std::vector<EnemyAssaultTrooper*> m_Enemies;

	Point2f m_Position;
	float m_Range;
	float m_DespawnRange;
	Rectf m_Window;

	int m_MaxSpawned;
	int m_CurrentAmount;

	int m_Score;
	int m_KillScore;

	float m_SpawnDelay;

	Timer* m_SpawnTimer;
	ResourceManager* m_ResourceManager;

	bool m_IsInRange;

	void SpawnEnemy();
	void CheckForDeath();
};

