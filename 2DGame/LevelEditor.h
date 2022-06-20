#pragma once
#include "Camera.h"
#include <vector>

#include "MouseButton.h"
#include "OptionSelector.h"

#include "FileWriter.h"
#include "SaveFile.h"
#include "PopUp.h"

#include "SVGParser.h"

#include "ReadFolder.h"

class LevelEditor final
{
public:
	LevelEditor(Rectf window, ResourceManager* resourceManager);
	~LevelEditor();
	LevelEditor(const LevelEditor& other) = delete;
	LevelEditor& operator=(const LevelEditor& other) = delete;
	LevelEditor(LevelEditor&& other) = delete;
	LevelEditor& operator=(LevelEditor&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void saveFileName(std::string name);

	void AddActor(Actor* actor);
	void ResetLevelEditor();
	void ReloadButtons();

private:
	ResourceManager* m_ResourceManager;

	int m_AmountOfTabs{ 2 };
	enum class ButtonTab { objects, spawners };
	ButtonTab m_CurrentButtonTab{ ButtonTab::objects };

	Camera* m_Camera;
	Rectf m_Window;
	Rectf m_FollowPoint;

	FileWriter* m_FileWriter;

	ReadFolder* m_MapsFolder;
	ReadFolder* m_SVGFolder;
	SVGParser* m_LevelSVG;
	std::vector<std::vector<Point2f>> m_Vertices;
	Point2f m_MapOffset;
	float m_MapScale;

	bool m_RemovedObject;

	bool m_Debug;

	MouseButton* m_SaveButton;

	MouseButton* m_TabLeft;
	MouseButton* m_TabRight;

	OptionSelector* m_MapSelector;
	std::vector<std::string> m_Maps;
	int m_CurrentMap;
	Texture* m_MapTexture;


	SaveFile* m_Saver;
	PopUp* m_PopUp;

	bool m_Saving;

	std::string m_SaveFileName;

	std::vector<MouseButton*> m_ObjectButtons;
	std::vector<MouseButton*> m_SpawnerButtons;
	bool m_HasPlacedPlayer;
	bool m_Moving;

	std::vector<Actor*> m_Actors;

	Point2f getMousePos() const;

	void CheckForRemoveActor();
	void CheckForNewActor();

	void NextButtonTab();
	void PreviousButtonTab();

	void CheckForPlayer();
	void DrawMapTexture() const;
	void SVGMapLoading(std::string SVGfilePath);

	void AddButtons();
};

//Objects
//Tree = 0
//House = 1
//Sandbag = 2
//Crate = 3

//Spawners
//EnemySpawner = 0
//Player = 1

