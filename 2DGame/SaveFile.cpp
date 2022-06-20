#include "pch.h"
#include "SaveFile.h"


SaveFile::SaveFile(Rectf window)
	: m_Window{ window }
	, m_Name{" "}
	, m_KeyIsPressed{ false }
	, m_KeyDelay{ new Timer{} }
	, m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
{
	updateText();

	m_KeyDelay->setTimeGoal(0.1f);
	m_Text = new Texture{ "Level name:", m_FontPath, 25, Color4f{0.8f, 0.8f, 0.8f, 1} };
}

SaveFile::~SaveFile()
{
	delete m_Nametext;
	m_Nametext = nullptr;
	
	delete m_Text;
	m_Text = nullptr;

	delete m_KeyDelay;
	m_KeyDelay = nullptr;
}

void SaveFile::updateText()
{
	m_Nametext = new Texture{ m_Name, m_FontPath, 25, Color4f{1, 1, 1, 1} };
}

void SaveFile::Update(float elapsedSec)
{
	size_t nameSize{ m_Name.size() };

	if (m_KeyIsPressed == false)
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		for (int i = int(SDL_SCANCODE_A); i <= int(SDL_SCANCODE_Z); i++)
		{
			if (pStates[i])
			{
				m_Name.push_back(i - SDL_SCANCODE_A + 'A');
				break;
			}
		}
		if (pStates[SDL_SCANCODE_BACKSPACE])
		{
			if (m_Name.size() > 0)
			{
				m_Name.pop_back();
			}
		}
		if (nameSize != m_Name.size())
		{
			m_KeyIsPressed = true;
		}
	}
	else
	{
		m_KeyDelay->Update(elapsedSec);
		if (m_KeyDelay->isTimerDone())
		{
			m_KeyIsPressed = false;
			m_KeyDelay->resetTimer();
		}
	}

	updateText();
}

void SaveFile::Draw() const
{
	utils::SetColor(Color4f{ 0.7f, 0.7f, 0.7f, 0.7f });
	utils::FillRect(Rectf{ getWindowCenter().x - 125, getWindowCenter().y - 50, 250, 100});
	m_Text->Draw(Point2f{ getWindowCenter().x - m_Text->GetWidth() / 2, getWindowCenter().y + 10 });
	m_Nametext->Draw(Point2f{ getWindowCenter().x - m_Nametext->GetWidth() / 2, getWindowCenter().y - m_Nametext->GetHeight()});
}


Point2f SaveFile::getWindowCenter() const
{
	float x{ m_Window.left + m_Window.width / 2 };
	float y{ m_Window.bottom + m_Window.height / 2 };
	return Point2f{ x, y };
}
std::string SaveFile::GetName() const
{
	return m_Name;
}