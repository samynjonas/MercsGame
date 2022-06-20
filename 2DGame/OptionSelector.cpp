#include "pch.h"
#include "OptionSelector.h"

OptionSelector::OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, HorizontalPlacement horizontal, VerticalPlacement vertical, Rectf window, ResourceManager* resourceManager)
	: m_Options{ options }
	, m_Window{ window }
	, m_TextSize{ textSize }
	, m_ResourceManager{ resourceManager }
	, m_Position{ HorizontalAlligment(horizontal, window, textSize), VerticalAlligment(vertical, window, textSize)}
	, m_CurrentTabTexture{ new Texture{ options[0], "..\\Resources\\Fonts\\RetroFont.ttf", textSize, mainColor } }
	, m_Color{ mainColor }
	, m_CurrentIndex{ 0 }
{
	m_TabLeft	= new MouseButton{ Rectf{m_Position.x - m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 4, m_Position.y, float(textSize), float(textSize) }, "<", mainColor, 30, mainColor, window, 0 };
	m_TabRight	= new MouseButton{ Rectf{m_Position.x + m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 3, m_Position.y, float(textSize), float(textSize) }, ">", mainColor, 30, mainColor, window, 1 };
	UpdateText();
}

OptionSelector::OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, HorizontalPlacement horizontal, float vertical, Rectf window, ResourceManager* resourceManager)
	: m_Options{ options }
	, m_Window{ window }
	, m_TextSize{ textSize }
	, m_ResourceManager{ resourceManager }
	, m_Position{ HorizontalAlligment(horizontal, window, textSize), vertical }
	, m_CurrentTabTexture{ new Texture{ options[0], "..\\Resources\\Fonts\\RetroFont.ttf", textSize, mainColor } }
	, m_Color{ mainColor }
	, m_CurrentIndex{ 0 }
{
	m_TabLeft	= new MouseButton{ Rectf{m_Position.x - m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 4, m_Position.y, float(textSize), float(textSize) }, "<", mainColor, 30, mainColor, window, 0 };
	m_TabRight	= new MouseButton{ Rectf{m_Position.x + m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 3, m_Position.y, float(textSize), float(textSize) }, ">", mainColor, 30, mainColor, window, 1 };
	UpdateText();
}

OptionSelector::OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, float horizontal, VerticalPlacement vertical, Rectf window, ResourceManager* resourceManager)
	: m_Options{ options }
	, m_Window{ window }
	, m_CurrentIndex{ 0 }
	, m_Color{ mainColor }
	, m_TextSize{ textSize }
	, m_ResourceManager{ resourceManager }
	, m_Position{ horizontal, VerticalAlligment(vertical, window, textSize) }
	, m_CurrentTabTexture{ new Texture{ options[0], "..\\Resources\\Fonts\\RetroFont.ttf", textSize, mainColor } }
{
	m_TabLeft	= new MouseButton{ Rectf{m_Position.x - m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 4, m_Position.y, float(textSize), float(textSize) }, "<", mainColor, 30, mainColor, window, 0 };
	m_TabRight	= new MouseButton{ Rectf{m_Position.x + m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 3, m_Position.y, float(textSize), float(textSize) }, ">", mainColor, 30, mainColor, window, 1 };
	UpdateText();
}

OptionSelector::OptionSelector(std::vector<std::string>& options, int textSize, Color4f mainColor, Point2f pos, Rectf window, ResourceManager* resourceManager)
	: m_Options{ options }
	, m_Position{ pos }
	, m_Window{ window }
	, m_CurrentIndex{ 0 }
	, m_Color{ mainColor }
	, m_TextSize{ textSize }
	, m_ResourceManager{ resourceManager }
	, m_CurrentTabTexture{ new Texture{ options[0], "..\\Resources\\Fonts\\RetroFont.ttf", textSize, mainColor } }
{
	m_TabLeft	= new MouseButton{ Rectf{m_Position.x - m_CurrentTabTexture->GetWidth() / 2 - float(textSize) * 4, m_Position.y, float(textSize), float(textSize) }, "<", mainColor, 30, mainColor, window, 0 };
	m_TabRight	= new MouseButton{ Rectf{m_Position.x + m_CurrentTabTexture->GetWidth() / 2 + float(textSize) * 3, m_Position.y, float(textSize), float(textSize) }, ">", mainColor, 30, mainColor, window, 1 };
	UpdateText();
}

OptionSelector::~OptionSelector()
{
	delete m_TabLeft;
	m_TabLeft = nullptr;

	delete m_TabRight;
	m_TabRight = nullptr;

	delete m_CurrentTabTexture;
	m_CurrentTabTexture = nullptr;

	m_ResourceManager = nullptr;
}

void OptionSelector::Draw(bool showBackGround) const
{
	m_TabLeft->Draw(false);
	m_TabRight->Draw(false);
	m_CurrentTabTexture->Draw(Point2f{ m_Position.x - m_CurrentTabTexture->GetWidth() / 2, m_Position.y});
}

void OptionSelector::Update()
{
	if (m_TabLeft->IsButtonPressed())
	{
		--m_CurrentIndex;
		if (m_CurrentIndex < 0)
		{
			m_CurrentIndex = int(m_Options.size()) - 1;
		}
		UpdateText();
	}
	if (m_TabRight->IsButtonPressed())
	{
		++m_CurrentIndex %= m_Options.size();
		UpdateText();
	}
}

void OptionSelector::setCurrentSelected(std::string text)
{
	for (size_t index = 0; index < m_Options.size(); index++)
	{
		if (text == m_Options[index])
		{
			m_CurrentIndex = int(index);
			UpdateText();
		}
	}
}

void OptionSelector::UpdateText()
{
	delete m_CurrentTabTexture;
	m_CurrentTabTexture = new Texture{ m_Options[m_CurrentIndex], "..\\Resources\\Fonts\\RetroFont.ttf", m_TextSize, m_Color };
	m_TabLeft->SetPosition(	Point2f{ m_Position.x - m_CurrentTabTexture->GetWidth() / 2 - float(m_TextSize) * 4, m_Position.y });
	m_TabRight->SetPosition(Point2f{ m_Position.x + m_CurrentTabTexture->GetWidth() / 2 + float(m_TextSize) * 3, m_Position.y });
}

float OptionSelector::VerticalAlligment(VerticalPlacement& vertical, Rectf window, int textSize) const
{
	float y{};
	switch (vertical)
	{
	case OptionSelector::VerticalPlacement::top:
		y = window.bottom + window.height - float(textSize) * 2;
		break;
	case OptionSelector::VerticalPlacement::center:
		y = window.bottom + window.height / 2;
		break;
	case OptionSelector::VerticalPlacement::bottom:
		y = window.bottom + textSize;
		break;
	}
	return y;
}

float OptionSelector::HorizontalAlligment(HorizontalPlacement& horizontal, Rectf window, int textSize) const
{
	float x{};
	switch (horizontal)
	{
	case OptionSelector::HorizontalPlacement::left:
		x = window.left + textSize * 7.5f;
		break;
	case OptionSelector::HorizontalPlacement::center:
		x = window.left + window.width / 2;
		break;
	case OptionSelector::HorizontalPlacement::right:
		x = window.left + window.width - float(textSize) * 2;
		break;
	}
	return x;
}

std::string OptionSelector::getCurrentText() const
{
	return m_Options[m_CurrentIndex];
}

int OptionSelector::getCurrentIndex() const
{
	return m_CurrentIndex;
}