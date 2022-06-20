#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

#include <iostream>

Sprite::Sprite( const std::string& filename, int nrCols, int nrRows, float secondsPerFrame, float scale)
	: m_Cols{nrCols}
	, m_Rows{nrRows}
	, m_SecondsPerFrame{ secondsPerFrame }
	, m_AccumulateSeconds{ }
	, m_CurrentFrame{ }
	, m_Scale{scale}
	, m_NrTotalFrames{ nrCols * nrRows }
	, m_MirrorVertical{ false }
	, m_MirrorHorizontal{ false }
	, m_CurrentRow{ 0 }
	, m_usesSpriteInfo{ false }
{
	m_Texture = new Texture{ filename };

	m_TextureFrameWidth	 = m_Texture->GetWidth()  / m_Cols;
	m_TextureFrameHeight = m_Texture->GetHeight() / m_Rows;
}

Sprite::Sprite(Texture* texture, int nrCols, int nrRows, float secondsPerFrame, float scale)
	: m_Cols{ nrCols }
	, m_Rows{ nrRows }
	, m_SecondsPerFrame{ secondsPerFrame }
	, m_AccumulateSeconds{ }
	, m_CurrentFrame{ }
	, m_Scale{ scale }
	, m_NrTotalFrames{ nrCols * nrRows }
	, m_MirrorVertical{ false }
	, m_MirrorHorizontal{ false }
	, m_CurrentRow{ 0 }
	, m_Texture{ texture }
	, m_usesSpriteInfo{ true }
{
	m_TextureFrameWidth = m_Texture->GetWidth() / m_Cols;
	m_TextureFrameHeight = m_Texture->GetHeight() / m_Rows;
}

Sprite::Sprite(SpriteInfo* spriteInfo)
	: m_Cols{ spriteInfo->collums }
	, m_Rows{ spriteInfo->rows }
	, m_SecondsPerFrame{ spriteInfo->fps }
	, m_AccumulateSeconds{ }
	, m_CurrentFrame{ }
	, m_Scale{ spriteInfo->scale }
	, m_NrTotalFrames{ spriteInfo->collums * spriteInfo->rows }
	, m_MirrorVertical{ false }
	, m_MirrorHorizontal{ false }
	, m_CurrentRow{ 0 }
	, m_Texture{ spriteInfo->texture }
	, m_usesSpriteInfo{ true }
{
	m_TextureFrameWidth = m_Texture->GetWidth() / m_Cols;
	m_TextureFrameHeight = m_Texture->GetHeight() / m_Rows;
}

Sprite::~Sprite()
{
	if (m_usesSpriteInfo == false)
	{
		delete m_Texture;
	}
	m_Texture = nullptr;
}

void Sprite::NextFrame()
{
	m_CurrentFrame++;
	m_CurrentFrame %= m_NrTotalFrames;
}
void Sprite::SwitchToFrame(const int frameNum, bool mirrorVertical, bool mirrorHorizontal)
{
	m_MirrorHorizontal = mirrorHorizontal;
	m_MirrorVertical = mirrorVertical;


	m_CurrentFrame = frameNum;
	m_CurrentFrame %= m_NrTotalFrames;
}
void Sprite::SwitchToRow(const int RowNum, bool mirrorVertical, bool mirrorHorizontal)
{
	m_MirrorHorizontal = mirrorHorizontal;
	m_MirrorVertical = mirrorVertical;

	m_CurrentRow = RowNum;
	++m_CurrentRow;
	m_CurrentRow %= m_Rows;
}

void Sprite::Update(float elapsedSec, CyleType cycleType)
{
	switch (cycleType)
	{
	case Sprite::CyleType::Cycle:
		m_AccumulateSeconds += elapsedSec;

		if (m_AccumulateSeconds >= m_SecondsPerFrame)
		{
			++m_CurrentFrame;
			m_CurrentFrame %= m_Cols;

			m_AccumulateSeconds = 0;
		}

		break;
	case Sprite::CyleType::SingleCycle:
		m_AccumulateSeconds += elapsedSec;
		if (m_AccumulateSeconds >= m_SecondsPerFrame && m_CurrentFrame < m_NrTotalFrames - 1)
		{
			++m_CurrentFrame;
			m_AccumulateSeconds = 0;
		}

		break;
	case Sprite::CyleType::Step:
		m_CurrentFrame %= m_NrTotalFrames;
		break;
	}
}
void Sprite::Draw(const Rectf& avatarRect)
{
	Rectf destinationRect{};
	destinationRect.left	= avatarRect.left;
	destinationRect.bottom	= avatarRect.bottom;
	destinationRect.width	= avatarRect.width  * m_Scale;
	destinationRect.height	= avatarRect.height * m_Scale;

	Rectf sourceRect{};
	sourceRect.left			= m_TextureFrameWidth * m_CurrentFrame;
	sourceRect.bottom		= m_TextureFrameHeight * m_CurrentRow;
	sourceRect.width		= m_TextureFrameWidth;
	sourceRect.height		= m_TextureFrameHeight;

	glPushMatrix();
		if (m_MirrorHorizontal == true)
		{
			glTranslatef(0, destinationRect.bottom, 0);
			glScalef(1, -1, 1);
			glTranslatef(0, -avatarRect.bottom - destinationRect.height, 0);
		}
		if (m_MirrorVertical == true)
		{
			glTranslatef(destinationRect.left, 0, 0);
			glScalef(-1, 1, 1);
			glTranslatef( -avatarRect.left - destinationRect.width, 0, 0);
		}

		m_Texture->Draw(destinationRect, sourceRect);	
	glPopMatrix();
}
void Sprite::Draw(const Rectf& avatarRect, const Rectf& source) const
{
	Rectf destinationRect{};
	destinationRect.left = avatarRect.left;
	destinationRect.bottom = avatarRect.bottom;
	destinationRect.width = avatarRect.width * m_Scale;
	destinationRect.height = avatarRect.height * m_Scale;

	float width{  source.width / m_Cols };
	float height{ source.height / m_Rows };

	Rectf sourceRect{};
	sourceRect.left = source.left + width * m_CurrentFrame;
	
	sourceRect.bottom = source.bottom - height * m_CurrentRow;
	sourceRect.width = width;
	sourceRect.height = height;

	glPushMatrix();
	if (m_MirrorHorizontal == true)
	{
		glTranslatef(0, destinationRect.bottom, 0);
		glScalef(1, -1, 1);
		glTranslatef(0, -avatarRect.bottom - destinationRect.height, 0);
	}
	if (m_MirrorVertical == true)
	{
		glTranslatef(destinationRect.left, 0, 0);
		glScalef(-1, 1, 1);
		glTranslatef(-avatarRect.left - destinationRect.width, 0, 0);
	}

	m_Texture->Draw(destinationRect, sourceRect);
	glPopMatrix();
}


Rectf Sprite::getSpriteRect() const
{
	return Rectf{ 0, 0, m_Texture->GetWidth(), m_Texture->GetHeight() };
}
Rectf Sprite::getTextureRect(const Rectf& source) const
{

	return Rectf{ 0, 0, (source.width * m_Scale) / m_Cols, (source.height * m_Scale) / m_Rows  };
}

bool Sprite::HasDoneCycle() const
{
	if (m_CurrentFrame >= m_NrTotalFrames - 1)
	{
		return true;
	}
	return false;
}

int Sprite::getCurrentFrame() const
{
	return m_CurrentFrame;
}
int Sprite::getCurrentRow() const
{
	return m_CurrentRow;
}
bool Sprite::getMirrorHorizontal() const
{
	return m_MirrorHorizontal;
}
bool Sprite::getMirrorVertical() const
{
	return m_MirrorVertical;
}
