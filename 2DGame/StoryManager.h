#pragma once
#include "ResourceManager.h"
#include "FileReader.h"

class StoryManager final
{
public:
	StoryManager(Rectf window, ResourceManager* resourceManager);
	~StoryManager();
	StoryManager(const StoryManager& other) = delete;
	StoryManager& operator=(const StoryManager& other) = delete;
	StoryManager(StoryManager&& other) = delete;
	StoryManager& operator=(StoryManager&& other) = delete;

	void Update();
	void Draw() const;
	void LoadLevel();

	bool HasLoadedLevel() const;
	bool HasAddedAll() const;

	std::vector<Actor*> GetLoadedActors();
	std::map<std::string, std::string> getMapPath();

	void nextLevel();

	void Reset();

private:
	Rectf m_Window;
	ResourceManager* m_ResourceManager;
	FileReader* m_FileReader;

	std::vector<std::string> m_StoryModeLevels;
	int m_CurrentLevel;

	bool m_LoadedLevel;
	bool m_AddedActors;
	bool m_AddedMap;

	void StoryLevels();
};

