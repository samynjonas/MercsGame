#pragma once
class Texture;

class Sprite final
{
public:
	enum class CyleType{ Cycle, SingleCycle, Step };

	Sprite(const std::string& filename, int nrCols, int nrRows, float secondsPerFrame, float scale);
	Sprite(Texture* texture, int nrCols, int nrRows, float secondsPerFrame, float scale);
	Sprite(SpriteInfo* spriteInfo);
	~Sprite();

	void Update(float elapsedSec, CyleType cycleType);
	void Draw(const Rectf& avatarRect);
	void Draw(const Rectf& avatarRect, const Rectf& source) const;


	void NextFrame();
	void SwitchToFrame( const int frameNum, bool mirrorVertical = false, bool mirrorHorizontal = false);	
	void SwitchToRow( const int rowNum, bool mirrorVertical = false, bool mirrorHorizontal = false);

	Rectf getSpriteRect() const;
	Rectf getTextureRect(const Rectf& source) const;
	bool HasDoneCycle() const;

	int getCurrentFrame() const;
	int getCurrentRow() const;

	bool getMirrorHorizontal() const;
	bool getMirrorVertical() const;

private:
	Texture* m_Texture;
	bool m_usesSpriteInfo;

	int m_Cols;
	int m_Rows;

	int m_CurrentRow;

	bool m_MirrorVertical;
	bool m_MirrorHorizontal;

	float m_SecondsPerFrame;
	float m_AccumulateSeconds;
	int m_CurrentFrame;
	const int m_NrTotalFrames;

	float m_Scale;

	float m_TextureFrameWidth{}, m_TextureFrameHeight{}; //A part of the sprite
};

