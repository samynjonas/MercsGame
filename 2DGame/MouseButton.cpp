#include "pch.h"
#include "MouseButton.h"

MouseButton::MouseButton(Rectf buttonRect, std::string text, Color4f textColor, int textSize, Color4f Buttoncolor, Rectf window, int info)
	: m_ButtonRect{ buttonRect }
	, m_Text{ text }
	, m_Window{ window }
	, m_Sprite{ nullptr }
	, m_IsHolding{ false }
	, m_IsPressed{ false }
	, m_Color{ Buttoncolor }
	, m_SpriteInfo{ nullptr }
	, m_InformationInt{ info }
	, m_MouseOnButton{ false }
	, m_IsButtonActive{ false }
	, m_Texture{ new Texture{ text, "..\\Resources\\Fonts\\RetroFont.ttf", textSize, textColor }}
{

}
MouseButton::MouseButton(Rectf buttonRect, Texture* texture, Rectf window, int info)
	: m_ButtonRect{ buttonRect }
	, m_Color{ Color4f{0, 0, 0, 0 } }
	, m_Window{ window }
	, m_IsPressed{ false }
	, m_Texture{ texture }
	, m_Sprite{ nullptr }
	, m_SpriteInfo{ nullptr }
	, m_IsHolding{ false }
	, m_InformationInt{ info }
	, m_MouseOnButton{ false }
	, m_Text{ " " }
	, m_IsButtonActive{ false }
{

}
MouseButton::MouseButton(Rectf buttonRect, SpriteInfo* spriteInfo, Rectf window, int info)
	: m_ButtonRect{ buttonRect }
	, m_Color{ Color4f{0, 0, 0, 0} }
	, m_Window{ window }
	, m_IsPressed{ false }
	, m_Texture{ nullptr }
	, m_SpriteInfo{ spriteInfo }
	, m_IsHolding{ false }
	, m_InformationInt{ info }
	, m_MouseOnButton{ false }
	, m_Text{ " " }
	, m_IsButtonActive{ false }
{
	m_Sprite = new Sprite{ spriteInfo };
}

MouseButton::~MouseButton()
{
	delete m_Texture;
	m_Texture = nullptr;

	delete m_Sprite;
	m_Sprite = nullptr;

	m_SpriteInfo = nullptr;
}

void MouseButton::Draw(bool showBackground = true ) const
{
	if (showBackground == true)
	{
		utils::SetColor(Color4f{ 0.8f, 0.8f, 0.8f, 0.3f });
		utils::FillRect(m_ButtonRect);
	}

	if (m_Texture == nullptr)
	{
		m_Sprite->Draw(m_ButtonRect, m_SpriteInfo->sourceRect);
	}
	else
	{
		utils::SetColor(m_Color);
		utils::FillRect(m_ButtonRect);

		m_Texture->Draw(m_ButtonRect);
	}

	int x, y;
	Uint32 mouse = { SDL_GetMouseState(&x,&y) };
	if (utils::IsPointInRect(Point2f{ float(x), float((m_Window.height - y)) }, m_ButtonRect) || m_IsButtonActive == true)
	{
		utils::SetColor(Color4f{ 1, 1, 1, 0.75f });
		utils::FillRect(m_ButtonRect);
	}
}

Point2f MouseButton::getMousePos() const
{
	int x, y;
	Uint32 mouse = { SDL_GetMouseState(&x,&y) };
	return Point2f{ float(x), float(m_Window.height - y) };
}
bool MouseButton::LeftMouse() const
{
	int x, y;
	Uint32 mouse;
	mouse = SDL_GetMouseState(&x, &y);

	if ((mouse & SDL_BUTTON_LMASK) != 0)
	{		
		return true;
	}
	return false;
}
bool MouseButton::RightMouse() const
{
	int x, y;
	Uint32 mouse;
	mouse = SDL_GetMouseState(&x, &y);

	if ((mouse & SDL_BUTTON_RMASK) != 0)
	{
		return true;
	}
	return false;
}

bool MouseButton::IsButtonPressed()
{
	if (m_IsPressed == false)
	{
		if (LeftMouse())
		{
			if (utils::IsPointInRect(getMousePos(), m_ButtonRect))
			{
				m_IsPressed = true;
				return true;
			}
			else
			{
				m_IsPressed = false;
			}
		}
		else
		{
			m_IsPressed = false;
		}
	}
	else
	{
		if (!LeftMouse())
		{
			m_IsPressed = false;
		}
		if (!utils::IsPointInRect(getMousePos(), m_ButtonRect))
		{
			m_IsPressed = false;
		}
	}
	return false;
}
bool MouseButton::IsButtonReleased()
{
	if (m_IsPressed == true)
	{
		if (!LeftMouse() && !RightMouse())
		{
			m_IsPressed = false;
			return true;
		}
	}
	return false;
}
bool MouseButton::IsHoldingMouse()
{
	if (LeftMouse())
	{
		if (utils::IsPointInRect(getMousePos(), m_ButtonRect))
		{
			m_IsHolding = true;
		}
	}
	else
	{
		m_IsHolding = false;
	}
	return m_IsHolding;
}

int MouseButton::GetButtonInfo() const
{
	return m_InformationInt;
}

void MouseButton::SetPosition(Point2f pos)
{
	m_ButtonRect.left = pos.x;
	m_ButtonRect.bottom = pos.y;
}
void MouseButton::SetHeight(float height)
{
	m_ButtonRect.height = height;
}
void MouseButton::SetWidth(float width)
{
	m_ButtonRect.width = width;
}

void MouseButton::UpdateSprite()
{
	delete m_Sprite;
	if (m_SpriteInfo != nullptr)
	{
		m_Sprite = new Sprite{ m_SpriteInfo };
	}
}

Rectf MouseButton::GetRect() const
{
	return m_ButtonRect;
}

std::string MouseButton::GetButtonText() const
{
	return m_Text;
}

void MouseButton::Activate()
{
	m_IsButtonActive = true;
}
void MouseButton::DeActivate()
{
	m_IsButtonActive = false;
}