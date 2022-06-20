#pragma once
#include <vector>
#include "ResourceManager.h"

class Actor;
class Bullet;

class BulletManager final
{
public:
	enum class BulletType { blueBullets, TankBullet, yellowBullets, singleBomb, bigBomb, jetBullets, none };

	BulletManager( BulletType bulletType, ResourceManager* resourceManager);
	~BulletManager();

	BulletManager(const BulletManager& other) = delete;
	BulletManager& operator=(const BulletManager& other) = delete;
	BulletManager(BulletManager&& other) = delete;
	BulletManager& operator=(BulletManager&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void Shoot( Point2f spawnPoint, Vector2f direction, int amount, Point2f endPoint );
	void CheckHit(Actor* actor);

	void AddBullet(BulletType bulletType, Point2f startPoint, Vector2f direction, Point2f endPoint);

	bool isDoneShoot();

	BulletType getBulletType();

private:
	ResourceManager* m_ResourceManager;

	BulletType m_BulletType;

	bool m_HasFired;
	bool m_Wait;

	float m_normalTime;
	float m_TimeElasped;
	float m_TimeTillFire;

	int m_ShootAmount;
	int m_BulletCount;

	std::vector<Bullet*> m_pBullets;
	std::vector<Rectf> m_pHitExplosion;

};

