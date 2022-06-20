#pragma once
#include "Bullet.h"
#include "Timer.h"

class SingleBomb final : public Bullet
{
public:
	SingleBomb(Point2f startPoint, Vector2f direction, Point2f endPoint, ResourceManager* resourceManager);
	virtual ~SingleBomb();

	SingleBomb(const SingleBomb& other) = delete;
	SingleBomb& operator=(const SingleBomb& other) = delete;
	SingleBomb(SingleBomb&& other) = delete;
	SingleBomb& operator=(SingleBomb&& other) = delete;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	bool canTouch() const;

private:
	Point2f m_StartPoint;
	Point2f m_endPoint;
	float explosionSize;

	Timer* m_LifeTime;

	bool m_Falling;
	bool m_CanTouch;

	bool m_Destroyed;

	float Distance( Point2f p1, Point2f p2 ) const;
};

