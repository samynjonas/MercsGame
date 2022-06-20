#include "pch.h"
#include "HUD.h"

HUD::HUD()
	: m_LevelNumber{ 1 }
	, m_Score{ 0 }
	, m_Time{ 200 }
	, m_Bombs{ 0 }
	, m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
	, m_AccumSec{ }
	, m_MaxHealth{ 10 }
{
	m_Health = m_MaxHealth;

	m_MissionTexture	= new Texture{ std::string{"MISSION"},	m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_ScoreTexture		= new Texture{ std::string{"SCORE"},	m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_TimeTexture		= new Texture{ std::string{"TIME"},		m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_LifeTexture		= new Texture{ std::string{"LIFE"},		m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_BombsTexture		= new Texture{ {"..\\Resources\\HUD\\BombsHUD.png"} };

	std::string missionNumberText{ std::to_string(m_LevelNumber) };
	std::string ScoreText{ std::to_string(m_Score) };
	std::string timeText{ std::to_string(m_Time) };
	std::string bombText{ std::to_string(m_Bombs) };

	m_MissionNumberTexture = new Texture{ missionNumberText,	m_FontPath, 60, Color4f{1, 1, 1, 1} };
	m_ScoreValueTexture = new Texture{ ScoreText,			m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_TimeValueTexture = new Texture{ timeText,			m_FontPath, 50, Color4f{1, 1, 1, 1} };
	m_BombsAmountTexture = new Texture{ bombText,			m_FontPath, 30, Color4f{1, 1, 1, 1} };
}

HUD::~HUD()
{
	delete m_MissionTexture;
	m_MissionTexture = nullptr;

	delete m_MissionNumberTexture;
	m_MissionNumberTexture = nullptr;
	
	delete m_ScoreTexture;
	m_ScoreTexture = nullptr;
	
	delete m_ScoreValueTexture;
	m_ScoreValueTexture = nullptr;
	
	delete m_TimeTexture;
	m_TimeTexture = nullptr;
	
	delete m_TimeValueTexture;
	m_TimeValueTexture = nullptr;
	
	delete m_LifeTexture;
	m_LifeTexture = nullptr;
	
	delete m_BombsTexture;
	m_BombsTexture = nullptr;
	
	delete m_BombsAmountTexture;
	m_BombsAmountTexture = nullptr;
}

void HUD::Draw() const
{
	m_MissionTexture->Draw(	Point2f{ 650, 450 });
	m_MissionNumberTexture->Draw(Point2f{ 650, 375 });

	m_ScoreTexture->Draw( Point2f{ 650, 325 });
	m_ScoreValueTexture->Draw(Point2f{ 675, 275 });

	m_TimeTexture->Draw( Point2f{ 650, 225 });
	m_TimeValueTexture->Draw(Point2f{ 675, 175 });

	m_LifeTexture->Draw( Point2f{ 650, 125 });

	utils::SetColor(Color4f{ 0, 100 / 255.f, 0, 1 });
	utils::FillRect(Point2f{ 650, 75 }, float(100 / m_MaxHealth) * (m_Health + 1), 40);
	
	utils::SetColor(Color4f{ 1, 1, 1, 1 });
	utils::DrawRect(Point2f{ 650, 75 }, 100, 40, 2);

	m_BombsTexture->Draw( Point2f{ 650, 25 } );
	m_BombsAmountTexture->Draw( Point2f{ 650 + m_BombsTexture->GetWidth() + 5, 25});
}

void HUD::Update(float elapsedSec)
{
	m_AccumSec+= elapsedSec;
	if (m_AccumSec >= 1)
	{
		--m_Time;
		m_AccumSec = 0;
	}

	UpdateTextures();
}

void HUD::UpdateTextures()
{
	std::string missionNumberText{ std::to_string(m_LevelNumber) };
	std::string ScoreText{ std::to_string(m_Score) };
	std::string timeText{ std::to_string(m_Time) };
	std::string bombText{ std::to_string(m_Bombs)};

	ResetText();

	m_MissionNumberTexture	= new Texture{ missionNumberText,	m_FontPath, 60, Color4f{1, 1, 1, 1} };
	m_ScoreValueTexture		= new Texture{ ScoreText,			m_FontPath, 30, Color4f{1, 1, 1, 1} };
	m_TimeValueTexture		= new Texture{ timeText,			m_FontPath, 50, Color4f{1, 1, 1, 1} };
	m_BombsAmountTexture	= new Texture{ bombText,			m_FontPath, 30, Color4f{1, 1, 1, 1} };
}

void HUD::UpdateHealth(int health)
{
	m_Health = health;
}
void HUD::UpdateBombs(int bombs)
{
	m_Bombs = bombs;
}
void HUD::UpdateScore(int score)
{
	m_Score = score;
}

void HUD::SetMaxHealth(int maxHealth)
{
	m_MaxHealth = maxHealth;
}

int HUD::GetTime() const
{
	return m_Time;
}

void HUD::ResetText()
{
	delete m_MissionNumberTexture;
	m_MissionNumberTexture = nullptr;
	delete m_ScoreValueTexture;
	m_ScoreValueTexture = nullptr;
	delete m_TimeValueTexture;
	m_TimeValueTexture = nullptr;
	delete m_BombsAmountTexture;
	m_BombsAmountTexture = nullptr;
}

void HUD::reset()
{
	m_LevelNumber = 1;
	m_Score = 0;
	m_Time = 200;
	m_Bombs = 0;
	m_AccumSec = 0;
	m_MaxHealth = 10;
}