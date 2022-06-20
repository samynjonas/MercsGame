#include "pch.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(Rectf pauseRect, Color4f backgroundColor)
	: m_PauseRect{ pauseRect }
	, m_BackgroundColor{ backgroundColor }
	, m_IsButtonPressed{ false }
	, m_CurrentButton{ 0 }
	, m_IsContinuePressed{ false }
	, m_IsQuitToGamePressed{ false }
{
	float buttonWidth{ pauseRect.width / 3 };
	float buttonHeight{ buttonWidth / 5 };

	m_Buttons.push_back(new Button{ "Continue",		Rectf{ pauseRect.left + pauseRect.width / 2 - buttonWidth / 2, pauseRect.bottom + pauseRect.height / 5 * 3, buttonWidth, buttonHeight }, backgroundColor, Color4f{1, 1, 1, 1}, 30 });
	m_Buttons.push_back(new Button{ "Quit to menu", Rectf{ pauseRect.left + pauseRect.width / 2 - buttonWidth / 2, pauseRect.bottom + pauseRect.height / 5 * 1, buttonWidth, buttonHeight }, backgroundColor, Color4f{1, 1, 1, 1}, 30 });
}

PauseMenu::~PauseMenu()
{
	for (size_t index = 0; index < m_Buttons.size(); index++)
	{
		delete m_Buttons[index];
		m_Buttons[index] = nullptr;
	}
	m_Buttons.clear();
}

void PauseMenu::Update()
{
	ButtonSelector();
	CheckForButtonPress();
}

void PauseMenu::Draw() const
{
	utils::SetColor(m_BackgroundColor);
	utils::FillRect(m_PauseRect);

	for (const Button* button: m_Buttons)
	{
		button->Draw();
	}
}

void PauseMenu::AddButton(Button* button)
{
	m_Buttons.push_back(button);
}

void PauseMenu::ButtonSelector()
{
	if (m_Buttons.size() > 0)
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_DOWN] && m_IsButtonPressed == false || pStates[SDL_SCANCODE_S] && m_IsButtonPressed == false)
		{
			++m_CurrentButton;
			m_CurrentButton %= m_Buttons.size();
			m_IsButtonPressed = true;
		}
		else if (pStates[SDL_SCANCODE_UP] && m_IsButtonPressed == false || pStates[SDL_SCANCODE_Z] && m_IsButtonPressed == false || pStates[SDL_SCANCODE_W] && m_IsButtonPressed == false)
		{
			--m_CurrentButton;
			if (m_CurrentButton < 0)
			{
				m_CurrentButton = int(m_Buttons.size()) - 1;
			}
			m_IsButtonPressed = true;
		}
		else if (m_IsButtonPressed == true && !pStates[SDL_SCANCODE_DOWN] && !pStates[SDL_SCANCODE_UP] && !pStates[SDL_SCANCODE_S] && !pStates[SDL_SCANCODE_Z] && !pStates[SDL_SCANCODE_W])
		{
			m_IsButtonPressed = false;
		}

		for (SIZE_T index = 0; index < m_Buttons.size(); index++)
		{
			if (m_CurrentButton == index)
			{
				if (m_Buttons[index]->isEnabled() == false)
				{
					m_Buttons[index]->Enable();
				}
			}
			else
			{
				if (m_Buttons[index]->isEnabled())
				{
					m_Buttons[index]->Dissable();
				}
			}
		}
	}
}
void PauseMenu::CheckForButtonPress()
{
	for (size_t index = 0; index < m_Buttons.size(); index++)
	{
		if (m_Buttons[index]->isEnabled())
		{
			DeActivateAll();
			switch (index)
			{
			case 0:
				//Continue
				m_IsContinuePressed = true;
				break;
			case 1:
				//Quit to game
				m_IsQuitToGamePressed = true;
				break;
			}
		}
	}
}

void PauseMenu::DeActivateAll()
{
	m_IsContinuePressed = false;
	m_IsQuitToGamePressed = false;
}

bool PauseMenu::ContinueGame() const
{
	return m_IsContinuePressed;
}
bool PauseMenu::QuitToGame() const
{
	return m_IsQuitToGamePressed;
}