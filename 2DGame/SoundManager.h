#pragma once
#include "SoundEffect.h"
#include "ReadFolder.h"
#include "SoundStream.h"

#include <vector>
#include <map>

class SoundManager final
{
public:
	SoundManager(std::string soundFolderPath, std::string backgroundPath);
	~SoundManager();
	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	void LoadSounds();
	bool HasLoaded() const;

	void SelectSoundTheme(std::string soundTheme);

	void PlaySound(std::string name);

	SoundStream* getBackground();

	void PauseAll();
	void ResumeAll();
	void SetVolume( int value );
	int GetVolume() const;

	std::vector<std::string> getSoundThemes() const;
	std::string getCurrentSoundTheme() const;

private:
	ReadFolder* m_FolderReader;
	ReadFolder* m_SoundReader;

	std::string m_CurrentSoundTheme;

	std::string m_SoundFolderPath;

	std::vector<std::string> m_SoundThemes;
	//	   \/ folder name - - - - - Sound name - Sound path
	std::map <std::string, std::map<std::string, std::string> > m_SoundEffects;
	std::map < std::string, SoundEffect* > m_LoadedSounds;

	SoundStream* m_BackgroundSound;

	bool m_HasLoaded;
};