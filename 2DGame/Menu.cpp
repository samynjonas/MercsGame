#include "pch.h"
#include "Menu.h"

Menu::Menu(std::string texturePath, buttonBehaviour buttonBehaviour)
	: m_CurrentButton{ 0 }
	, m_ButtonBehaviour{ buttonBehaviour }
	, m_IsButtonPressed{ false }
	, m_PulsTimer{ new Timer{} }
	, m_ShowButton{ true }
	, m_BackGroundType{ backGroundType::texture }
	, m_BackGroundColor{ Color4f{1, 1, 1, 1} }
{
	m_BackgroundTexture = new Texture{ texturePath };
	m_PulsTimer->setTimeGoal(0.75f);
}
Menu::Menu(Color4f BackGroundcolor, buttonBehaviour buttonBehaviour)
	: m_CurrentButton{ 0 }
	, m_ButtonBehaviour{ buttonBehaviour }
	, m_IsButtonPressed{ false }
	, m_PulsTimer{ new Timer{} }
	, m_ShowButton{ true }
	, m_BackGroundType{ backGroundType::color }
	, m_BackGroundColor{ BackGroundcolor }
{
	m_BackgroundTexture = nullptr;
	m_PulsTimer->setTimeGoal(0.75f);
}

Menu::~Menu()
{
	for (SIZE_T index = 0; index < m_Buttons.size(); index++)
	{
		delete m_Buttons[index];
		m_Buttons[index] = nullptr;
	}

	delete m_BackgroundTexture;
	m_BackgroundTexture = nullptr;

	delete m_PulsTimer;
	m_PulsTimer = nullptr;
}

void Menu::Update(float elapsedSec)
{
	if (m_ButtonBehaviour == buttonBehaviour::pulsing)
	{
		m_PulsTimer->Update(elapsedSec);
		if (m_PulsTimer->isTimerDone())
		{
			m_ShowButton = !m_ShowButton;
			m_PulsTimer->resetTimer();
		}
	}

	ButtonSelector();	
}

void Menu::Draw( Rectf window ) const
{
	if (m_BackGroundType == backGroundType::texture)
	{
		m_BackgroundTexture->Draw(window);
	}
	else if (m_BackGroundType == backGroundType::color)
	{
		utils::SetColor(m_BackGroundColor);
		utils::FillRect(window);
	}

	if (m_ShowButton == true)
	{
		for (const Button* button : m_Buttons)
		{
			button->Draw();
		}
	}
}

void Menu::AddButton(std::string text, Rectf buttonRect, Color4f ButtonColor, Color4f textColor)
{
	m_Buttons.push_back(new Button{ text, buttonRect, ButtonColor, textColor, 30 });
}

void Menu::AddButton(std::string text, Button::HorizontalAlligment horizontal, float vertical, float width, float height, Color4f ButtonColor, Color4f textColor, Rectf window, int textSize)
{
	m_Buttons.push_back(new Button{ text, horizontal, vertical, width, height, ButtonColor, textColor, window, textSize });
}

void Menu::ButtonSelector()
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


		for (size_t index = 0; index < m_Buttons.size(); index++)
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

int Menu::getIndexActiveButton() const
{
	for (size_t index = 0; index < m_Buttons.size(); index++)
	{
		if (m_Buttons[index]->isEnabled())
		{
			return int(index);
		}
	}
	return 0;
}
std::string Menu::getTextActiveButton() const
{
	for (size_t index = 0; index < m_Buttons.size(); index++)
	{
		if (m_Buttons[index]->isEnabled())
		{
			return m_Buttons[index]->GetText();
		}
	}
	return " ";
}

void Menu::ButtonOffset(Point2f offset)
{
	
}
