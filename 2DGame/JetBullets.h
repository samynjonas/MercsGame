#pragma once
#include "Bullet.h"


class JetBullets final : public Bullet
{
public:
	JetBullets(Point2f startPoint, Vector2f direction, Point2f endPoint, ResourceManager* resourceManager);
	virtual ~JetBullets();

	JetBullets(const JetBullets& other) = delete;
	JetBullets& operator=(const JetBullets& other) = delete;
	JetBullets(JetBullets&& other) = delete;
	JetBullets& operator=(JetBullets&& other) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec) override;

private:
	Point2f m_EndPoint;

	void movement(float elapsedSec);

};

