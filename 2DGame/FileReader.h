#pragma once
#include "Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "EnemyAssaultTrooper.h"

#include "ResourceManager.h"

#include <map>
#include "ReadFolder.h"

class FileReader final
{
public:
	FileReader(std::string filePath, Rectf window, ResourceManager* resourceManager);
	~FileReader();
	FileReader(const FileReader& other) = delete;
	FileReader& operator=(const FileReader& other) = delete;
	FileReader(FileReader&& other) = delete;
	FileReader& operator=(FileReader&& other) = delete;

	std::vector<Actor*> GetActors() const;
	std::map<std::string, std::string> GetMapPath();

private:
	Rectf m_Window;
	std::vector<Actor*> m_Actors;

	std::map<std::string, std::string> m_Map;
	ReadFolder* m_MapReader;
	ReadFolder* m_SVGReader;
	
	ResourceManager* m_ResourceManager;

	void ReadPath(std::string filePath);
	void CreateActor(const std::string& actorElement);

	void createMap(const std::string& actorElement);

	void createPlayer(const std::string& actorElement);
	void createEnemySpawner(const std::string& actorElement);
	void CreateName(const std::string& actorElement);
	
	void CreateCrate(const std::string& actorElement);
	void CreateTree(const std::string& actorElement);
	void CreateHouse(const std::string& actorElement);
	void CreateSandbag(const std::string& actorElement);
	void CreateBoss(const std::string& actorElement);


	void testString(std::string string) const;

	Point2f ToPoint2f(const std::string& point2fStr) const;
	std::string GetAttributeValue(const std::string& name, const std::string& shape) const;
	int attributeLastIndex(const std::string& attrName, const std::string& element) const;
};

