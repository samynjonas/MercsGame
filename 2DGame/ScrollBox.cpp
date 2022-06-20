#include "pch.h"
#include "ScrollBox.h"

ScrollBox::ScrollBox(Rectf scrollBoxRect, Color4f backGroundColor, Rectf window)
	: m_Window{ window }
	, m_ActiveText{ " " }
	, m_ScrollBox{ scrollBoxRect }
	, m_BackgroundColor{ backGroundColor }
{
	m_ScrolBar = new MouseButton{ Rectf{ scrollBoxRect.left, scrollBoxRect.bottom + 1, scrollBoxRect.width / 15, m_ScrollBox.height - 1 }, " ", Color4f{0, 0, 0, 0}, 30, Color4f{0, 0, 0, 1}, window, 0 };
}

ScrollBox::~ScrollBox()
{
	for (size_t index = 0; index < m_AvailableButtons.size(); index++)
	{
		delete m_AvailableButtons[index];
		m_AvailableButtons[index] = nullptr;
	}
	m_AvailableButtons.clear();

	delete m_ScrolBar;
	m_ScrolBar = nullptr;
}

void ScrollBox::Update()
{
	//Changing scrollbar pos
	if (m_ScrolBar->GetRect().bottom >= m_ScrollBox.bottom && m_ScrolBar->GetRect().bottom + m_ScrolBar->GetRect().height <= m_ScrollBox.bottom + m_ScrollBox.height)
	{
		if (m_ScrolBar->IsHoldingMouse())
		{
			m_ScrolBar->SetPosition(Point2f{ m_ScrolBar->GetRect().left, GetMousePos().y - m_ScrolBar->GetRect().height / 2});
		}
	}
	
	//If the scroll bar is lower then the bottom, stop
	if (m_ScrolBar->GetRect().bottom < m_ScrollBox.bottom)
	{
		m_ScrolBar->SetPosition(Point2f{ m_ScrolBar->GetRect().left, m_ScrollBox.bottom });
	}

	//If the scroll bar is higher then the top, stop
	if (m_ScrolBar->GetRect().bottom + m_ScrolBar->GetRect().height > m_ScrollBox.bottom + m_ScrollBox.height)
	{
		m_ScrolBar->SetPosition(Point2f{ m_ScrolBar->GetRect().left, m_ScrollBox.bottom + m_ScrollBox.height - m_ScrolBar->GetRect().height });
	}

	float BarOffset{ (m_ScrollBox.bottom + m_ScrollBox.height) - (m_ScrolBar->GetRect().bottom + m_ScrolBar->GetRect().height) };

	for (MouseButton* mouseButton : m_AvailableButtons)
	{
		float height{ m_ScrollBox.height / 6 };
		float width{ m_ScrollBox.width - m_ScrolBar->GetRect().width };
		Point2f pos{ m_ScrollBox.left + m_ScrolBar->GetRect().width, ( m_ScrollBox.bottom + m_ScrollBox.height - height * mouseButton->GetButtonInfo() ) + BarOffset };
		mouseButton->SetPosition(pos);
	}

	if (utils::IsPointInRect(GetMousePos(), m_ScrollBox))
	{
		for (MouseButton* mouseButton : m_AvailableButtons)
		{
			if (mouseButton->IsButtonPressed())
			{
				DeActivateAllButtons();
				mouseButton->Activate();
				m_ActiveText = mouseButton->GetButtonText();
			}
		}
	}	
}
void ScrollBox::Draw() const
{
	utils::SetColor(m_BackgroundColor);
	utils::FillRect(m_ScrollBox);

	m_ScrolBar->Draw(true);

	for (const MouseButton* mouseButton : m_AvailableButtons)
	{
		mouseButton->Draw(true);
	}

	utils::SetColor(Color4f{ 0.0f, 0.4f, 0.4f, 1.0f });

	utils::FillRect(Rectf{ 0, 0, m_Window.width, m_ScrollBox.bottom });
	utils::FillRect(Rectf{ 0, 0, m_ScrollBox.left, m_Window.height });
	utils::FillRect(m_Window.left + m_Window.width - m_ScrollBox.left + m_ScrollBox.width, 0, m_Window.width, m_Window.height);
	utils::FillRect(m_Window.left, m_Window.bottom + m_ScrollBox.bottom + m_ScrollBox.height, m_Window.width, m_Window.height);
}

void ScrollBox::DeActivateAllButtons()
{
	for (MouseButton* mouseButton : m_AvailableButtons)
	{
		mouseButton->DeActivate();
	}
}
void ScrollBox::AddButton(std::string text, Color4f textColor, Color4f Buttoncolor, Rectf window)
{
	float height{ m_ScrollBox.height / 6 };
	float width{ m_ScrollBox.width - m_ScrolBar->GetRect().width };
	Point2f pos{ m_ScrollBox.left + m_ScrolBar->GetRect().width, m_ScrollBox.bottom + m_ScrollBox.height - height * (m_AvailableButtons.size() + 1)};

	Rectf buttonRect{ pos.x, pos.y, width, height };

	m_AvailableButtons.push_back( new MouseButton{ buttonRect, text, textColor, 30, Buttoncolor, window,  int(m_AvailableButtons.size())});

	if (m_AvailableButtons.size() * buttonRect.height > m_ScrollBox.height + height)
	{
		m_ScrolBar->SetHeight( m_ScrolBar->GetRect().height - height);
		m_ScrolBar->SetPosition(Point2f{ m_ScrolBar->GetRect().left, m_ScrollBox.bottom + m_ScrollBox.height - m_ScrolBar->GetRect().height });
	}
}

int ScrollBox::GetButtonInfo()
{
	for (MouseButton* mouseButton : m_AvailableButtons)
	{
		if (mouseButton->IsButtonPressed())
		{
			return mouseButton->GetButtonInfo();
		}
	}
	return -1;
}
Point2f ScrollBox::GetMousePos() const
{
	int x, y;
	Uint32 mouse = { SDL_GetMouseState(&x,&y) };
	return Point2f{ float(x), float(m_Window.height - y) };
}
std::string ScrollBox::GetButtonText()
{
	return m_ActiveText;
}
bool ScrollBox::IsAButtonPressed() const
{
	if (m_ActiveText != " ")
	{
		return true;
	}
	return false;
}