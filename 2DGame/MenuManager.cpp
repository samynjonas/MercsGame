#include "pch.h"
#include "MenuManager.h"

MenuManager::MenuManager(ResourceManager* resourceManager, Rectf window)
	: m_CurrentScreen{ Screens::start }
	, m_GoToScreen{ Screens::start }
	, m_IsButtonPressed{ false }
	, m_Transition{ new Transition{ .25f } }
	, m_ActivateTransition{ false }
	, m_ActivateScene{ false }
	, m_PressedButton{ Buttons::none }
	, m_ResourceManager{ resourceManager }
{
	m_StartScreen = new Menu{ "..\\Resources\\IntroScene\\MercsScene3\\54.jpg", Menu::buttonBehaviour::pulsing };
	m_StartScreen->AddButton( "Press enter to start", Rectf{ 150, 50, 200, 50 }, Color4f{ 1, 1, 1, 0 }, Color4f{ 255 / 255.f, 204 / 255.f, 0.f, 1.f });	
	
	m_GameSelectScreen = new Menu{ Color4f{.5f, .5f,.5f,1.f }, Menu::buttonBehaviour::frozen };
	m_GameSelectScreen->AddButton( "STORY MODE",		Button::HorizontalAlligment::center, 350, 200, 50, Color4f{ 1, 1, 1, 0 }, Color4f{ 1, 1, 1, 1 }, window, 50);
	m_GameSelectScreen->AddButton( "LEVEL SELECTOR",	Button::HorizontalAlligment::center, 300, 200, 50, Color4f{ 1, 1, 1, 0 }, Color4f{ 1, 1, 1, 1 }, window, 50);
	m_GameSelectScreen->AddButton( "OPTIONS",			Button::HorizontalAlligment::center, 250, 200, 50, Color4f{ 1, 1, 1, 0 }, Color4f{ 1, 1, 1, 1 }, window, 50);

	m_OptionScreen = new OptionMenu(Color4f{ .5f, .5f,.5f,1.f }, Color4f{ 1, 1, 1, 0 }, Menu::buttonBehaviour::frozen, resourceManager, window);
}

MenuManager::~MenuManager()
{
	m_ResourceManager = nullptr;

	delete m_StartScreen;
	m_StartScreen = nullptr;

	delete m_GameSelectScreen;
	m_GameSelectScreen = nullptr;
	
	delete m_OptionScreen;
	m_OptionScreen = nullptr;

	delete m_Transition;
	m_Transition = nullptr;
}

void MenuManager::Update(float elapsedSec)
{
	switch (m_CurrentScreen)
	{
	case MenuManager::Screens::start:
		m_StartScreen->Update(elapsedSec);
		break;
	case MenuManager::Screens::selection:
		m_GameSelectScreen->Update(elapsedSec);
		break;
	case MenuManager::Screens::options:
		m_OptionScreen->Update(elapsedSec);
		break;
	}

	if (m_ActivateTransition)
	{
		m_Transition->Update(elapsedSec);
		if (m_Transition->HalfTransition())
		{
			m_CurrentScreen = m_GoToScreen;
		}
		if (m_Transition->IsTransitionComplete())
		{
			m_ActivateTransition = false;
			m_Transition->ResetTransition();
		}
	}
}

void MenuManager::ReceiveInput()
{
	if (m_ActivateTransition == false)
	{
		switch (m_CurrentScreen)
		{
		case MenuManager::Screens::start:
			m_GoToScreen = Screens::selection;
			m_ActivateTransition = true;
			break;
		case MenuManager::Screens::selection:
			if (m_GameSelectScreen->getIndexActiveButton() == 0)
			{
				//Start Story mode
				m_PressedButton = Buttons::StoryMode;
				m_ActivateScene = true;
				m_ActivateTransition = true;
			}
			else if (m_GameSelectScreen->getIndexActiveButton() == 1)
			{
				//Start level editor
				m_PressedButton = Buttons::levelSelection;
				m_ActivateScene = true;
				m_ActivateTransition = true;
			}
			else if (m_GameSelectScreen->getIndexActiveButton() == 2)
			{
				m_GoToScreen = Screens::options;
				m_PressedButton = Buttons::options;
				m_ActivateTransition = true;
			}
			break;
		case MenuManager::Screens::options:
			if (m_OptionScreen->isBackPressed())
			{
				m_OptionScreen->Reset();
				m_GoToScreen = Screens::selection;
				m_ActivateTransition = true;
			}
			break;
		}
	}
}

void MenuManager::Draw(Rectf window) const
{
	switch (m_CurrentScreen)
	{
	case MenuManager::Screens::start:
		m_StartScreen->Draw(window);		
		break;
	case MenuManager::Screens::selection:
		m_GameSelectScreen->Draw(window);
		break;
	case MenuManager::Screens::options:
		m_OptionScreen->Draw(window);
		break;
	}

	if (m_ActivateTransition)
	{
		m_Transition->Draw(Color4f{ 0, 0, 0, 1 }, window);
	}
}

void MenuManager::ResetButton()
{
	m_PressedButton = Buttons::none;
	m_ActivateScene = false;
	m_Transition->ResetTransition();
}
bool MenuManager::isInOptions() const
{
	if (m_CurrentScreen == Screens::options)
	{
		return true;
	}
	return false;
}
MenuManager::Buttons MenuManager::ActivatedButton() const
{
	if (m_ActivateScene == true)
	{
		return m_PressedButton;
	}
	return Buttons::none;
}