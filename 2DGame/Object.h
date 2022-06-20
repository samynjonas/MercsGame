#pragma once
#include "Actor.h"
#include "SpriteManager.h"

class Object final: public Actor
{
public:
	enum class objectType { Tree, House, Crate, SandbagWall };
	enum class CrateType { Power, Food, Health, Bomb, none };

	Object(Rectf actorRect, Rectf window, ResourceManager* resourceManager, objectType objectType, CrateType crateType = CrateType::none);
	virtual ~Object();

	Object(const Object& other) = delete;
	Object& operator=(const Object& other) = delete;
	Object(Object&& other) = delete;
	Object& operator=(Object&& other) = delete;
	
	virtual void Update(float elapsedSec, const Level* level) override;

	objectType getObjectType() const;
	CrateType getCrateType() const;

	std::vector<Point2f> getVertices() const;
	Point2f getCenter() const;

	virtual void KillActor();

private:
	objectType m_ObjectType;
	CrateType m_CrateType;
	int m_Health;

	void InitializeSprite();
};

