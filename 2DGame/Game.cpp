#include "pch.h"
#include "Game.h"
#include "Texture.h"

Game::Game(const Window& window)
	: m_Window{ window }
	, m_TVScreen{ float(m_Window.width / 4.f), float(m_Window.height / 4.f * 1), float(m_Window.width / 4.f * 2.f), float(m_Window.width / 4.f * 1.5f) }
	, m_IsGamePaused{ false }
	, m_QuitGame{ false }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}
void Game::Cleanup( )
{
	delete m_ResourceManager;
	m_ResourceManager = nullptr;

	delete m_MenuManager;
	m_MenuManager = nullptr;

	delete m_PauseMenu;
	m_PauseMenu = nullptr;

	delete m_Level;
	m_Level = nullptr;
	
	delete m_LevelEditor;
	m_LevelEditor = nullptr;
	
	delete m_LevelSelector;
	m_LevelSelector = nullptr;

	delete m_StoryMode;
	m_StoryMode = nullptr;
}

void Game::Initialize( )
{
	m_ResourceManager = new ResourceManager{ "..\\Resources\\Sounds", "..\\Resources\\BackgroundSound\\background.mp3", Rectf{0, 0, m_Window.width, m_Window.height} };
}

void Game::LoadResources()
{
	//After initializing all sprites
	m_MenuManager = new MenuManager{ m_ResourceManager, Rectf{0, 0, m_Window.width, m_Window.height} };
	m_LevelSelector = new LevelSelector{ Rectf{0, 0, m_Window.width, m_Window.height }, m_ResourceManager };
	m_LevelEditor = new LevelEditor{ Rectf{0, 0, m_Window.width, m_Window.height}, m_ResourceManager };
	m_Level = new Level{ m_TVScreen, 0.5f, m_ResourceManager };
	m_StoryMode = new StoryManager{ Rectf{0, 0, m_Window.width, m_Window.height}, m_ResourceManager };

	float pauseWidth{	m_Window.width	/ 2.5f };
	float pauseHeight{	m_Window.height / 2.5f };

	m_PauseMenu = new PauseMenu{ Rectf{ m_Window.width / 2 - pauseWidth / 2, m_Window.height / 2 - pauseHeight / 2, pauseWidth, pauseHeight }, Color4f{ 0.4f, 0.4f, 0.4f, 0.95f } };
}

void Game::Update( float elapsedSec )
{
	m_ResourceManager->Update(elapsedSec);
	if (!m_IsGamePaused)
	{
		gameStateChanger();

		switch (m_CurrentGameState)
		{
		case Game::gameState::loading:
			m_ResourceManager->LoadResources();
			break;
		case Game::gameState::startUp:
			m_ResourceManager->Sounds()->getBackground()->Play(0);

			m_ResourceManager->IntroScenes()->Update(elapsedSec);
			break;
		case Game::gameState::menus:
			m_MenuManager->Update(elapsedSec);
			break;
		case Game::gameState::storyMode:
			m_Level->Update(elapsedSec);
			if (m_Level->HasKilledBoss())
			{				
				m_Level->Reset();
				m_CurrentGameState = gameState::menus;

				m_MenuManager->ResetButton();
				
				/*
				m_StoryMode->nextLevel();
				m_Level->Reset();
				m_StoryMode->LoadLevel();
				for (size_t index = 0; index < m_StoryMode->GetLoadedActors().size(); index++)
				{
					m_Level->ImportActor(m_StoryMode->GetLoadedActors()[index]);
				}
				m_Level->LoadMap(m_StoryMode->getMapPath());
				*/				
			}
			else if (m_Level->isGameOver() || m_Level->HasKilledBoss() )
			{
				m_Level->Reset();
				m_CurrentGameState = gameState::menus;

				m_MenuManager->ResetButton();
			}
			break;
		case Game::gameState::levelEditor:
			m_LevelEditor->Update(elapsedSec);
			break;
		case Game::gameState::levelSelector:
			m_LevelSelector->Update(elapsedSec);
			if (m_LevelSelector->HasLoadLevel())
			{
				for (Actor* actors : m_LevelSelector->GetLoadedActors())
				{
					m_Level->ImportActor(actors);
				}
				m_Level->LoadMap(m_LevelSelector->getMapPath());
				m_CurrentGameState = gameState::storyMode;
			}
			else if (m_LevelSelector->HasLoadEditor())
			{
				m_LevelEditor->saveFileName(m_LevelSelector->GetFileName());
				for (Actor* actors : m_LevelSelector->GetLoadedActors())
				{
					m_LevelEditor->AddActor(actors);
				}
				m_Level->LoadMap(m_LevelSelector->getMapPath());
				m_CurrentGameState = gameState::levelEditor;
			}
			else if (m_LevelSelector->NeedNewFile())
			{
				m_LevelEditor->ResetLevelEditor();
				m_CurrentGameState = gameState::levelEditor;
			}
			break;
		}
	}
	else
	{
		//Game is paused
		m_PauseMenu->Update();		
	}	
}
void Game::Draw( ) const
{
	ClearBackground();
	switch (m_CurrentGameState)
	{
	case Game::gameState::loading:
		m_ResourceManager->Draw();
		break;
	case Game::gameState::startUp:
		m_ResourceManager->IntroScenes()->Draw(Rectf{0,0,m_Window.width, m_Window.height});
		break;
	case Game::gameState::menus:
		m_MenuManager->Draw(Rectf{ 0,0,m_Window.width, m_Window.height });
		break;
	case Game::gameState::storyMode:
		m_Level->Draw();
		break;
	case Game::gameState::levelEditor:
		m_LevelEditor->Draw();
		break;
	case Game::gameState::levelSelector:
		m_LevelSelector->Draw();
		break;
	}

	if (m_IsGamePaused)
	{
		m_PauseMenu->Draw();
	}

	m_ResourceManager->Debug()->Draw();
}

void Game::gameStateChanger()
{
	if (m_CurrentGameState == gameState::loading)
	{
		if (m_ResourceManager->HasLoaded())
		{
			LoadResources();
			m_CurrentGameState = gameState::startUp;
		}
	}
	else if (m_CurrentGameState == gameState::startUp)
	{
		if (m_ResourceManager->IntroScenes()->isDone())
		{
			m_CurrentGameState = gameState::menus;
		}
	}
	else if (m_CurrentGameState == gameState::menus)
	{
		if (m_MenuManager->ActivatedButton() == MenuManager::Buttons::StoryMode)
		{
			m_StoryMode->Reset();
			m_Level->Reset();

			m_StoryMode->LoadLevel();
			for (size_t index = 0; index < m_StoryMode->GetLoadedActors().size(); index++)
			{
				m_Level->ImportActor(m_StoryMode->GetLoadedActors()[index]);
			}
			m_Level->LoadMap(m_StoryMode->getMapPath());

			m_CurrentGameState = gameState::storyMode;
		}
		else if (m_MenuManager->ActivatedButton() == MenuManager::Buttons::LevelEditor)
		{
			m_CurrentGameState = gameState::levelEditor;
		}
		else if (m_MenuManager->ActivatedButton() == MenuManager::Buttons::levelSelection)
		{
			m_LevelSelector->Reset();
			m_CurrentGameState = gameState::levelSelector;
		}
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}
void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_RETURN:
		if (m_CurrentGameState == gameState::startUp)
		{
			m_ResourceManager->FreeIntroScene();
			m_CurrentGameState = gameState::menus;
		}
		else if (m_CurrentGameState == gameState::menus)
		{
			m_MenuManager->ReceiveInput();
		}
		else if (m_IsGamePaused)
		{
			if (m_PauseMenu->ContinueGame())
			{
				m_IsGamePaused = false;
				m_PauseMenu->DeActivateAll();
			}
			else if (m_PauseMenu->QuitToGame())
			{
				if (m_CurrentGameState == gameState::storyMode)
				{
					m_Level->Reset();
				}
				else if (m_CurrentGameState == gameState::levelEditor)
				{
					m_LevelEditor->ResetLevelEditor();
				}
				m_CurrentGameState = gameState::menus;
				m_IsGamePaused = false;

				m_MenuManager->ResetButton();
				m_PauseMenu->DeActivateAll();
			}
		}

		break;
	case SDLK_ESCAPE:
		switch (m_CurrentGameState)
		{
		case Game::gameState::storyMode:
		case Game::gameState::levelEditor:
		case Game::gameState::levelSelector:
			m_IsGamePaused = !m_IsGamePaused;
			break;
		}
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}
void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
}
void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	if (m_CurrentGameState == gameState::menus)
	{
		if (m_MenuManager->isInOptions())
		{
			m_MenuManager->ReceiveInput();
		}
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.4f, 0.4f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

bool Game::QuitGame() const
{
	return m_QuitGame;
}
