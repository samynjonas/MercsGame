#pragma once
class Sprite;
#include "SpriteManager.h"
#include "ResourceManager.h"

class Bullet
{
public:
	enum class BulletType { blueBullets, TankBullet, yellowBullets, singleBomb, bigBomb, none };

	Bullet(Point2f startPoint, ResourceManager* resourceManager);
	virtual ~Bullet();
	
	Bullet(const Bullet& other) = delete;
	Bullet& operator=(const Bullet& other) = delete;
	Bullet(Bullet&& other) = delete;
	Bullet& operator&(Bullet&& other) = delete;


	virtual void Update(float elapsedSec);
	virtual void Draw() const = 0;

	void MakeReadyToDestroy();
	bool isReadyToDestroy() const;
	bool isDestroyin() const;

	Point2f getPosition() const;
	Rectf getRect() const;

	BulletType getBulletType() const;

protected:
	ResourceManager* m_ResourceManager;

	Rectf m_BulletRect;
	Vector2f m_Speed;

	BulletType m_BulletType;

	SpriteInfo* m_SpriteInfo;
	Sprite* m_Sprite;
	SpriteInfo* m_BulletHitSpriteInfo;
	Sprite* m_BulletHitSprite;

	bool AwaitDestroy;
	bool Destroy;
private:

};

