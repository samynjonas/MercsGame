#pragma once
#include "SoundManager.h"
#include "SpriteManager.h"
#include "ResourceLoader.h"
#include "IntroSceneManager.h"
#include "Debugger.h"

class ResourceManager final
{
public:
	ResourceManager( std::string SoundFolderPath, std::string backgroundPath, Rectf window);
	~ResourceManager();
	ResourceManager(const ResourceManager& other) = delete;
	ResourceManager& operator=(const ResourceManager& other) = delete;
	ResourceManager(ResourceManager&& other) = delete;
	ResourceManager& operator=(ResourceManager&& other) = delete;

	void LoadResources();
	void FreeIntroScene();

	void Update(float elapsedSec);
	void Draw() const;

	bool HasLoaded() const;

	SoundManager* Sounds();
	SpriteManager* Sprites();
	IntroSceneManager* IntroScenes();
	Debugger* Debug();

private:
	SoundManager* m_SoundManager;
	SpriteManager* m_SpriteManager;
	ResourceLoader* m_ResourceLoader;
	IntroSceneManager* m_IntroScenes;
	Debugger* m_Debugger;

	bool m_HasLoaded;
	bool m_PlayBackgroundSound;
};

