#include "pch.h"
#include "Object.h"
#include "Sprite.h"
#include <vector>

Object::Object(Rectf actorRect, Rectf window, ResourceManager* resourceManager, objectType objectType, CrateType crateType )
	: Actor{ actorRect, window, 0, resourceManager }
	, m_ObjectType{ objectType }
	, m_Health{ 6 }
	, m_CrateType{ crateType }
{
	m_ActorType = actorType::object;

	switch (m_ObjectType)
	{
	case Object::objectType::Tree:
		m_Health = 4;
		break;
	case Object::objectType::House:
		m_Health = 2 * 3;
		break;
	case Object::objectType::Crate:
		m_Health = 2;
		break;
	case Object::objectType::SandbagWall:
		//Indestructable
		break;
	}
	InitializeSprite();
}


Object::~Object()
{

}

void Object::Update(float elapsedSec, const Level* level)
{
	if (m_ObjectType == objectType::Tree)
	{
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
	}
	else
	{
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::Step);
	}
}

Object::objectType Object::getObjectType() const
{
	return m_ObjectType;
}

Object::CrateType Object::getCrateType() const
{
	return m_CrateType;
}


std::vector<Point2f> Object::getVertices() const
{
	std::vector<Point2f> vertices;
	vertices.push_back(Point2f{ m_ActorRect.left, m_ActorRect.bottom });
	vertices.push_back(Point2f{ m_ActorRect.left + m_ActorRect.width, m_ActorRect.bottom });
	vertices.push_back(Point2f{ m_ActorRect.left + m_ActorRect.width, m_ActorRect.bottom + m_ActorRect.height });
	vertices.push_back(Point2f{ m_ActorRect.left, m_ActorRect.bottom + m_ActorRect.height });
	vertices.push_back(Point2f{ m_ActorRect.left, m_ActorRect.bottom });

	return vertices;
}

void Object::KillActor()
{
	if (m_ObjectType != Object::objectType::SandbagWall)
	{
		if (m_Health > 0)
		{
			--m_Health;
			if (m_IdleSprite->HasDoneCycle())
			{
				m_IdleSprite->SwitchToFrame(0);
			}
		}
		else
		{
			//Delete objects
			m_IsActorDead = true;
		}
	}	

	switch (m_ObjectType)
	{
	case Object::objectType::Tree:
		break;
	case Object::objectType::House:
		//std::cout << abs((m_Health - 9) / 3) << std::endl;
		m_IdleSprite->SwitchToFrame( abs((m_Health - 7) / 3) );
		break;
	case Object::objectType::Crate:
		m_IdleSprite->NextFrame();
		//std::cout << "spawn pickup" << std::endl;
		break;
	case Object::objectType::SandbagWall:
		//Indestructable
		break;
	}
}

Point2f Object::getCenter() const
{
	return Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 };
}

void Object::InitializeSprite()
{
	switch (m_ObjectType)
	{
	case Object::objectType::Tree:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Tree");
		break;
	case Object::objectType::House:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("House");
		break;
	case Object::objectType::Crate:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Crates");
		break;
	case Object::objectType::SandbagWall:
		m_IdleSpriteInfo = m_ResourceManager->Sprites()->GetSprite("Sandbag");
		break;
	}

	delete m_IdleSprite;
	m_IdleSprite = new Sprite{ m_IdleSpriteInfo->texture, m_IdleSpriteInfo->collums, m_IdleSpriteInfo->rows, m_IdleSpriteInfo->fps, m_IdleSpriteInfo->scale };

	m_ActorRect.width	= m_IdleSprite->getTextureRect(m_IdleSpriteInfo->sourceRect).width;
	m_ActorRect.height	= m_IdleSprite->getTextureRect(m_IdleSpriteInfo->sourceRect).height;

}


