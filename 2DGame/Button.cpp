#include "pch.h"
#include "Button.h"

Button::Button(std::string text, Rectf buttonRect, Color4f ButtonColor, Color4f textColor, int textSize)
	: m_Text{ text }
	, m_ButtonRect{ buttonRect }
	, m_ButtonColor{ ButtonColor }
	, m_IsActive{ false }
	, m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
	, m_ActiveColor{	Color4f{textColor.r, textColor.g, textColor.b, 1 } }
	, m_NonActiveColor{ Color4f{textColor.r, textColor.g, textColor.b, 0.5f } }
{
	m_TextTexture = new Texture{ m_Text, m_FontPath, textSize, m_NonActiveColor };
}

Button::Button(std::string text, HorizontalAlligment horizontal, float vertical, float width, float height, Color4f ButtonColor, Color4f textColor, Rectf window, int textSize)
	: m_Text{ text }
	, m_ButtonColor{ ButtonColor }
	, m_IsActive{ false }
	, m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
	, m_ActiveColor{ Color4f{textColor.r, textColor.g, textColor.b, 1 } }
	, m_NonActiveColor{ Color4f{textColor.r, textColor.g, textColor.b, 0.5f } }
{
	m_TextTexture = new Texture{ m_Text, m_FontPath, textSize, m_NonActiveColor };

	m_ButtonRect = Rectf(getHorizontalPos(horizontal, window, textSize, width), vertical, width, height);
}

Button::~Button()
{
	delete m_TextTexture;
	m_TextTexture = nullptr;
}

void Button::Draw() const
{
	utils::SetColor(m_ButtonColor);
	utils::FillRect(m_ButtonRect);
	m_TextTexture->Draw(m_ButtonRect);
}

void Button::UpdateColor()
{
	delete m_TextTexture;
	m_TextTexture = nullptr;

	if (m_IsActive)
	{
		m_TextTexture = new Texture{ m_Text, m_FontPath, 30, m_ActiveColor };
	}
	else
	{
		m_TextTexture = new Texture{ m_Text, m_FontPath, 30, m_NonActiveColor };
	}
}

void Button::Enable()
{
	m_IsActive = true;
	UpdateColor();
}
void Button::Dissable()
{
	m_IsActive = false;
	UpdateColor();
}

void Button::SetPosition(Point2f pos)
{
	m_ButtonRect.left = pos.x;
	m_ButtonRect.bottom = pos.y;
}

Rectf Button::GetRect() const
{
	return m_ButtonRect;
}
bool Button::isEnabled() const
{
	return m_IsActive;
}
std::string Button::GetText() const
{
	return m_Text;
}

float Button::getHorizontalPos(HorizontalAlligment& horizontal, Rectf window, int textSize, float width) const
{
	float x{};
	switch (horizontal)
	{
	case HorizontalAlligment::left:
		x = window.left + textSize * 7.5f;
		break;
	case HorizontalAlligment::center:
		x = window.left + (window.width / 2) - width / 2;
		break;
	case HorizontalAlligment::right:
		x = window.left + window.width - float(textSize) * 2;
		break;
	}
	return x;
}
