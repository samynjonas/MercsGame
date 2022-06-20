#include "pch.h"
#include "PopUp.h"


PopUp::PopUp(Rectf rect, std::string popUpText, Color4f color, Rectf window)
	: m_PopUpRect{ rect }
	, m_Color{ color }
	, m_Show{ false }
	, m_Window{ window }
{
	m_PopUpText = new Texture{ popUpText, "..\\Resources\\Fonts\\RetroFont.ttf", 10,  Color4f{1, 1, 1, 1} };
	m_OkButton = new MouseButton{ Rectf{ rect.left + 10, rect.bottom + 5, rect.width - 20, 20 }, "   OK   ", Color4f{1, 1, 1, 1}, 30, Color4f{1, 0, 0, 1}, window, 0 };
}

PopUp::~PopUp()
{
	delete m_OkButton;
	m_OkButton = nullptr;

	delete m_PopUpText;
	m_PopUpText = nullptr;
}

void PopUp::Draw() const
{
	if (m_Show == true)
	{
		utils::SetColor(Color4f{ 0.25f, 0.25f , 0.25f, 0.5f });
		utils::FillRect(m_Window);


		utils::SetColor(m_Color);
		utils::FillRect(m_PopUpRect);

		m_PopUpText->Draw(Point2f{ Center().x - m_PopUpText->GetWidth() / 2, Center().y });

		m_OkButton->Draw(true);
	}	
}
bool PopUp::IsOkPressed() const
{
	if (m_Show == true)
	{
		return m_OkButton->IsButtonPressed();
	}
	return false;
}

void PopUp::Pop()
{
	m_Show = true;
}
void PopUp::Close()
{
	m_Show = false;
}

Point2f PopUp::Center() const
{
	return Point2f{ m_PopUpRect.left + m_PopUpRect.width / 2, m_PopUpRect.bottom + m_PopUpRect.height / 2 };
}

bool PopUp::IsActive() const
{
	return m_Show;
}