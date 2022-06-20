#pragma once
#include "Actor.h"
#include "Player.h"
#include "ResourceManager.h"

class BossClass final
{
public:
	enum class BossType { Jet, Tank };

	BossClass( Rectf bossRoom, BossType boss, Rectf window, ResourceManager* resourceManager);
	~BossClass();

	BossClass(const BossClass& other) = delete;
	BossClass& operator=(const BossClass& other) = delete;
	BossClass(BossClass&& other) = delete;
	BossClass& operator=(BossClass&& other) = delete;

	void Update(float elapsedSec, const Level* level, Player* playerOne, Player* playerTwo);
	void Draw() const;

	Point2f getMiddlePoint() const;
	bool isActorInRoom(const Player* player);
	bool isBossActive() const;

	bool IsActorDead() const;

	BossType getBossType() const;

private:
	ResourceManager* m_ResourceManager;

	Actor* m_Boss;

	Rectf m_BossRoom;
	BossType m_CurrentBoss;

	bool m_SpawnBoss;
};

