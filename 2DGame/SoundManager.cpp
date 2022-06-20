#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager(std::string soundFolderPath, std::string backgroundPath)
	: m_SoundFolderPath{ soundFolderPath }
	, m_FolderReader{	new ReadFolder{} }
	, m_SoundReader{	new ReadFolder{} }
	, m_BackgroundSound{ new SoundStream{ backgroundPath } }
	, m_HasLoaded{ false }
	, m_CurrentSoundTheme{ "" }
{

}

SoundManager::~SoundManager()
{
	delete m_FolderReader;
	m_FolderReader = nullptr;

	delete m_SoundReader;
	m_SoundReader = nullptr;

	for (auto& map : m_LoadedSounds)
	{
		delete map.second;
		map.second = nullptr;
	}
	m_LoadedSounds.clear();
}

void SoundManager::LoadSounds()
{
	m_FolderReader->SetFolderPath(m_SoundFolderPath);
	for (auto& map : m_FolderReader->GetFolderContent())
	{
		m_SoundThemes.push_back(map.first);
		m_SoundReader->SetFolderPath(map.second);

		m_SoundEffects[map.first] = m_SoundReader->GetFolderContent();
	}
	SelectSoundTheme(m_SoundThemes[0]);
	m_HasLoaded = true;
}

bool SoundManager::HasLoaded() const
{
	return m_HasLoaded;
}

void SoundManager::SelectSoundTheme(std::string soundTheme)
{
	bool foundTheme{ false };

	for (size_t index = 0; index < m_SoundThemes.size(); index++)
	{
		if (m_SoundThemes[index] == soundTheme)
		{
			foundTheme = true;
			m_CurrentSoundTheme = soundTheme;
		}
	}
	if (foundTheme == false)
	{
		soundTheme = m_SoundThemes[0];
	}

	for (auto& map : m_SoundEffects[soundTheme])
	{
		m_LoadedSounds[map.first] = new SoundEffect{ map.second };
	}
}

void SoundManager::PlaySound(std::string name)
{
	if (m_LoadedSounds.count(name))
	{
		//std::cout << "contains key" << std::endl;
		m_LoadedSounds[name]->Play(0);
	}
}

void SoundManager::PauseAll()
{
	for (auto& map : m_LoadedSounds)
	{
		map.second->PauseAll();
		break;
	}
}
void SoundManager::ResumeAll()
{
	for (auto& map : m_LoadedSounds)
	{
		map.second->ResumeAll();
		break;
	}
}
void SoundManager::SetVolume(int value)
{
	for (auto& map : m_LoadedSounds)
	{
		map.second->SetVolume(value);
	}
}
int SoundManager::GetVolume() const
{
	for (auto& map : m_LoadedSounds)
	{
		 return map.second->GetVolume();
	}
	return 0;
}

std::vector<std::string> SoundManager::getSoundThemes() const
{
	return m_SoundThemes;
}

std::string SoundManager::getCurrentSoundTheme() const
{
	return m_CurrentSoundTheme;
}

SoundStream* SoundManager::getBackground()
{
	return m_BackgroundSound;
}
