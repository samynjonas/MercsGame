#pragma once
#include <map>
#include <vector>

#include "Button.h"
#include "MouseButton.h"
#include "ScrollBox.h"

#include "FileReader.h"
#include "ReadFolder.h"

#include "Menu.h"

class LevelSelector final
{
public:
	LevelSelector(Rectf window, ResourceManager* resourceManager);
	~LevelSelector();
	LevelSelector(const LevelSelector& other) = delete;
	LevelSelector& operator=(const LevelSelector& other) = delete;
	LevelSelector(LevelSelector&& other) = delete;
	LevelSelector& operator=(LevelSelector&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	bool HasLoadLevel() const;
	bool HasLoadEditor() const;
	bool NeedNewFile() const;

	std::string GetFileName() const;

	std::vector<Actor*> GetLoadedActors() const;
	std::map<std::string, std::string> getMapPath();

	void Reset();

private:
	std::vector<std::string> m_Keys;
	std::map<std::string, std::string> m_LevelFiles;
	
	FileReader* m_FileReader;
	ReadFolder* m_FolderReader;

	std::string m_FontPath;

	ResourceManager* m_ResourceManager;

	Rectf m_Window;
	ScrollBox* m_ScrollBox;

	MouseButton* m_Edit;
	MouseButton* m_Play;
	MouseButton* m_Create;

	bool m_IsButtonPressed;
	bool m_LoadedLevel;
	bool m_LoadedEditFile;
	bool m_CreateNewFile;

	void SearchForFiles();
	void InitializeKeys();
	void ReadMap() const;
	void ReadKeys();
	void AddToScroll();
};

