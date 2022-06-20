#pragma once
#include "Bullet.h"
class YellowBullets final : public Bullet
{
public:
	YellowBullets(Point2f startPoint, Vector2f direction, ResourceManager* resourceManager);
	virtual ~YellowBullets();

	YellowBullets(const YellowBullets& other) = delete;
	YellowBullets& operator=(const YellowBullets& other) = delete;
	YellowBullets(YellowBullets&& other) = delete;
	YellowBullets& operator=(YellowBullets&& other) = delete;

	//virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:
};

