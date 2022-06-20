#pragma once
#include "Menu.h"
#include "OptionMenu.h"

#include "Transition.h"

#include "ResourceManager.h"
#include <vector>

class MenuManager final
{
public:
	enum class Screens { start, selection, options, none };
	enum class Buttons{ none, options, StoryMode, LevelEditor, levelSelection };

	MenuManager(ResourceManager* resourceManager, Rectf window);
	~MenuManager();

	MenuManager(const MenuManager& other) = delete;
	MenuManager& operator=(const MenuManager& other) = delete;
	MenuManager(MenuManager&& other) = delete;
	MenuManager& operator=(MenuManager&& other) = delete;
	
	void Update(float elapsedSec);
	void Draw(Rectf window) const;

	Buttons ActivatedButton() const;

	void ReceiveInput();
	void ResetButton();

	bool isInOptions() const;

private:
	ResourceManager* m_ResourceManager;

	Screens m_CurrentScreen;
	Screens m_GoToScreen;

	Buttons m_PressedButton;

	bool m_IsButtonPressed;
	bool m_ActivateTransition;

	bool m_ActivateScene;

	Menu* m_StartScreen;
	Menu* m_GameSelectScreen;
	OptionMenu* m_OptionScreen;

	Transition* m_Transition;
};

