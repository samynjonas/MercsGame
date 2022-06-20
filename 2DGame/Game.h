#pragma once
#include <vector>
#include "Timer.h"

#include "MenuManager.h"
#include "PauseMenu.h"

#include "FileReader.h"

#include "ResourceManager.h"

#include "Level.h"
#include "LevelEditor.h"
#include "LevelSelector.h"
#include "StoryManager.h"

class ArcadeMachine;
class Camera;
class Actor;
class Player;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

	bool QuitGame() const;

private:
	// DATA MEMBERS
	const Window m_Window;
	Rectf m_TVScreen; //Screen ratio 4:3
	
	enum class gameState { loading, startUp, menus, storyMode, levelEditor, levelSelector };
	gameState m_CurrentGameState{ gameState::loading };

	MenuManager* m_MenuManager;
	PauseMenu* m_PauseMenu;

	Level* m_Level;
	LevelEditor* m_LevelEditor;
	LevelSelector* m_LevelSelector;
	StoryManager* m_StoryMode;

	ResourceManager* m_ResourceManager;

	bool m_IsGamePaused;
	bool m_QuitGame;

	// FUNCTIONS
	void LoadResources();
	void gameStateChanger();

	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};