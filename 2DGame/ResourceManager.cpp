#include "pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(std::string SoundFolderPath, std::string backgroundPath, Rectf window)
	: m_SoundManager{ new SoundManager{ SoundFolderPath, backgroundPath } }
	, m_SpriteManager{ new SpriteManager{} }
	, m_IntroScenes{ new IntroSceneManager{} }
	, m_HasLoaded{ false }
	, m_Debugger{ new Debugger{ window } }
	, m_PlayBackgroundSound{ false }

{
	m_ResourceLoader = new ResourceLoader{ m_SpriteManager, m_IntroScenes, Rectf{0, 0, window.width, window.height } };
}

ResourceManager::~ResourceManager()
{
	delete m_SoundManager;
	m_SoundManager = nullptr;

	delete m_SpriteManager;
	m_SpriteManager = nullptr;

	delete m_ResourceLoader;
	m_ResourceLoader = nullptr;

	delete m_IntroScenes;
	m_IntroScenes = nullptr;

	delete m_Debugger;
	m_Debugger = nullptr;
}

void ResourceManager::FreeIntroScene()
{
	if (m_Debugger->IsEnabled())
	{
		std::cout << "Free memory" << std::endl;
	}

	delete m_IntroScenes;
	m_IntroScenes = nullptr;
}

void ResourceManager::LoadResources()
{
	if (m_SoundManager->HasLoaded() == false)
	{
		m_SoundManager->LoadSounds();
	}
	else if(m_ResourceLoader->HasLoaded() == false)
	{
		m_ResourceLoader->Load();
	}
	else if (m_ResourceLoader->HasLoaded())
	{
		m_HasLoaded = true;
	}
}
bool ResourceManager::HasLoaded() const
{
	return m_HasLoaded;
}

void ResourceManager::Update(float elapsedSec)
{
	m_Debugger->Update(elapsedSec);
	if (m_SoundManager->getBackground()->IsPlaying())
	{
		m_PlayBackgroundSound = true;
	}
	if (m_PlayBackgroundSound == true && !m_SoundManager->getBackground()->IsPlaying())
	{
		m_SoundManager->getBackground()->Play(0);
	}


}

void ResourceManager::Draw() const
{
	m_ResourceLoader->Draw();
}

SoundManager* ResourceManager::Sounds()
{
	return m_SoundManager;
}
SpriteManager* ResourceManager::Sprites()
{
	return m_SpriteManager;
}
IntroSceneManager* ResourceManager::IntroScenes()
{
	return m_IntroScenes;
}
Debugger* ResourceManager::Debug()
{
	return m_Debugger;
}