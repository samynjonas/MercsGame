#include "pch.h"
#include "ArcadeMachine.h"

#include "utils.h"
#include "Texture.h"

ArcadeMachine::ArcadeMachine( Rectf screenSize )
	: m_TVScreen{ screenSize }
{
	m_Joysticktextures = new Texture{ "..\\Resources\\HUD\\Joysticks.png" };

	m_JoyStickBox = new Rectf[m_AmountofJoyStick];
	m_JoystickVertical = new VerticalAxis[m_AmountofJoyStick];
	m_JoyStickHorizontal = new HorizontalAxis[m_AmountofJoyStick];
	m_JoystickBallPos = new Point2f[m_AmountofJoyStick];

	for (int index = 0; index < m_AmountofJoyStick; index++)
	{
		m_JoyStickBox[index] = Rectf{ screenSize.left + 50 + (index * screenSize.width / 2), screenSize.bottom - 75, 50, 50 };
		m_JoystickVertical[index] = VerticalAxis::middle;
		m_JoyStickHorizontal[index] = HorizontalAxis::middle;
	}

	m_ButtonsPos = new Point2f[m_AmountOfButtons];
	m_ButtonPressed = new bool[m_AmountOfButtons];

	for (int index = 0; index < m_AmountOfButtons; index++)
	{
		if (index < m_AmountOfButtons / 2)
		{
			m_ButtonsPos[index] = Point2f{ m_JoyStickBox[0].left + m_JoyStickBox[0].width + (25 * (index + 1)), m_JoyStickBox[0].bottom + m_JoyStickBox[0].height - 10 };
		}
		else
		{
			m_ButtonsPos[index] = Point2f{ m_JoyStickBox[1].left + m_JoyStickBox[1].width + (25 * (index + 1 - m_AmountOfButtons / 2)), m_JoyStickBox[0].bottom + m_JoyStickBox[0].height - 10 };
		}
	}
}

ArcadeMachine::~ArcadeMachine()
{
	delete m_Joysticktextures;
	m_Joysticktextures = nullptr;

	delete[] m_JoystickVertical;
	m_JoystickVertical = nullptr;

	delete[] m_JoyStickHorizontal;
	m_JoyStickHorizontal = nullptr;
	
	delete[] m_JoystickBallPos;
	m_JoystickBallPos = nullptr;

	delete[] m_ButtonsPos;
	m_ButtonsPos = nullptr;
	
	delete[] m_ButtonPressed;
	m_ButtonPressed = nullptr;
	
	delete[] m_JoyStickBox;
	m_JoyStickBox = nullptr;
}

void ArcadeMachine::Draw() const
{
	DrawMachine();

	Color4f pressed{ 1,0,0,1 };
	Color4f Notpressed{ 0,1,0,1 };

	for (int index = 0; index < m_AmountOfButtons; index++)
	{
		if (m_ButtonPressed[index] == true)
		{
			utils::SetColor(pressed);
		}
		else
		{
			utils::SetColor(Notpressed);
		}
		utils::FillEllipse(m_ButtonsPos[index], 10, 10);
	}

	for (int index = 0; index < m_AmountofJoyStick; index++)
	{
		utils::SetColor(Color4f{ 0, 0.4f, 0.6f, 1 });
		utils::DrawRect(m_JoyStickBox[index]);

		m_Joysticktextures->Draw( m_JoyStickBox[index], 
			Rectf{ ( int(m_JoyStickHorizontal[index]) + 1 ) * m_Joysticktextures->GetWidth()  / 3, 
				   m_Joysticktextures->GetHeight() / 3 - ( int(m_JoystickVertical[index])   + 2 ) * m_Joysticktextures->GetHeight() / 3,
			       m_Joysticktextures->GetWidth()  / 3, 
			       m_Joysticktextures->GetHeight() / 3 });
	}
}

void ArcadeMachine::update(float elapsedSec)
{
	for (int index = 0; index < m_AmountOfButtons; index++)
	{
		m_ButtonPressed[index] = false;
	}

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_E])
	{
		m_ButtonPressed[0] = true;
	}
	if (pStates[SDL_SCANCODE_R])
	{
		m_ButtonPressed[1] = true;
	}
	if (pStates[SDL_SCANCODE_O])
	{
		m_ButtonPressed[2] = true;
	}
	if (pStates[SDL_SCANCODE_P])
	{
		m_ButtonPressed[3] = true;
	}

	for (int index = 0; index < m_AmountofJoyStick; index++)
	{
		m_JoystickVertical[index] = VerticalAxis::middle;
		m_JoyStickHorizontal[index] = HorizontalAxis::middle;
	}

	if (pStates[SDL_SCANCODE_Z] || pStates[SDL_SCANCODE_W])
	{
		m_JoystickVertical[0] = VerticalAxis::up;
	}
	else if (pStates[SDL_SCANCODE_S])
	{
		m_JoystickVertical[0] = VerticalAxis::down;
	}
	if (pStates[SDL_SCANCODE_Q] || pStates[SDL_SCANCODE_A])
	{
		m_JoyStickHorizontal[0] = HorizontalAxis::left;
	}
	else if (pStates[SDL_SCANCODE_D])
	{
		m_JoyStickHorizontal[0] = HorizontalAxis::right;
	}

	if (pStates[SDL_SCANCODE_U])
	{
		m_JoystickVertical[1] = VerticalAxis::up;
	}
	else if (pStates[SDL_SCANCODE_J])
	{
		m_JoystickVertical[1] = VerticalAxis::down;
	}
	if (pStates[SDL_SCANCODE_H])
	{
		m_JoyStickHorizontal[1] = HorizontalAxis::left;
	}
	else if (pStates[SDL_SCANCODE_K])
	{
		m_JoyStickHorizontal[1] = HorizontalAxis::right;
	}

	for (int index = 0; index < m_AmountofJoyStick; index++)
	{
		m_JoystickBallPos[index] = Point2f{ (m_JoyStickBox[index].left + m_JoyStickBox[index].width / 2) + m_JoyStickBox[index].width / 3 * int(m_JoyStickHorizontal[index]), (m_JoyStickBox[index].bottom + m_JoyStickBox[index].height / 2) + m_JoyStickBox[index].height / 3 * int(m_JoystickVertical[index]) };
	}
}

void ArcadeMachine::DrawMachine() const
{
	utils::SetColor(Color4f{ 0, 0, 0.2f ,1 });
	utils::FillRect(Rectf{ 0, 0, m_TVScreen.left,  800 });
	utils::FillRect(Rectf{ m_TVScreen.left + m_TVScreen.width, 0, 800, 800 });
	utils::FillRect(Rectf{ 0, m_TVScreen.bottom + m_TVScreen.height, 800, 200 });
	utils::FillRect(Rectf{ 0, 0, 800, m_TVScreen.bottom });

	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::DrawRect(m_TVScreen);
}


int ArcadeMachine::getHorizontalP1() const
{
	return int(m_JoyStickHorizontal[0]);
}
int ArcadeMachine::getHorizontalP2() const
{
	return int(m_JoyStickHorizontal[1]);
}
int ArcadeMachine::getVerticalP1() const
{
	return int(m_JoystickVertical[0]);
}
int ArcadeMachine::getVerticalP2() const
{
	return int(m_JoystickVertical[1]);
}