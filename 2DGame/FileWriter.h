#pragma once
#include "Actor.h"
#include "Object.h"
#include "BossClass.h"
#include "JetBoss.h"

#include <vector>

class FileWriter final
{
public:
	FileWriter( std::string mapPath );
	~FileWriter();
	FileWriter(const FileWriter& other) = delete;
	FileWriter& operator=(const FileWriter& other) = delete;
	FileWriter(FileWriter&& other) = delete;
	FileWriter& operator=(FileWriter&& other) = delete;

	void Write(const std::vector<Actor*> actors, std::string levelName, std::string mapName);

private:
	std::string m_MapPath;

	std::string TypeToString( Actor::actorType actorType ) const;
	std::string TypeToString( Object::objectType objectType ) const;

	std::string Point2fToString(const Actor* actor) const;
	std::string dropToString(const Object* object) const;
	std::string BossTypeToString(const BossClass* boss) const;
	std::string widthToString(const Actor* actor) const;
	std::string heightToString(const Actor* actor) const;
};

