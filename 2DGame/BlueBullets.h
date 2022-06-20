#pragma once
#include "Bullet.h"
#include "SpriteManager.h"

class BlueBullets final : public Bullet
{
public:
	BlueBullets( Point2f startPoint, Vector2f direction, ResourceManager* resourceManager);
	virtual ~BlueBullets();

	BlueBullets(const BlueBullets& other) = delete;
	BlueBullets& operator=(const BlueBullets& other) = delete;
	BlueBullets(BlueBullets&& other) = delete;
	BlueBullets& operator=(BlueBullets&& other) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:

};

