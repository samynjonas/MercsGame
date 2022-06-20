#pragma once
#include "Texture.h"

class HUD final
{
public:
	HUD();
	~HUD();
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& other) = delete;


	void Update(float elapsedSec);
	void Draw() const;

	void UpdateHealth( int health );
	void UpdateBombs( int bombs );
	void UpdateScore(int score);

	void SetMaxHealth(int maxHealth);

	int GetTime() const;

	void reset();

private:
	int m_LevelNumber;
	int m_Score;

	int m_Time;
	float m_AccumSec;

	int m_Health;
	int m_MaxHealth;
	
	int m_Bombs;

	std::string m_FontPath;

	//Text
	Texture* m_MissionTexture;
	Texture* m_MissionNumberTexture;

	Texture* m_ScoreTexture;
	Texture* m_ScoreValueTexture;

	Texture* m_TimeTexture;
	Texture* m_TimeValueTexture;

	Texture* m_LifeTexture;

	Texture* m_BombsTexture;
	Texture* m_BombsAmountTexture;


	void UpdateTextures();

	void ResetText();
};

