#include "pch.h"
#include "ResourceLoader.h"
#include "SpriteManager.h"
#include "IntroSceneManager.h"

ResourceLoader::ResourceLoader(SpriteManager* spriteManager, IntroSceneManager* introSceneManager, Rectf window)
	: m_SpriteManager{ spriteManager }
	, m_IntroScenes{ introSceneManager }
	, m_Window{ window }
	, m_DoneLoading{ false }
	, m_LoadBar{ Rectf{ window.left + 25, window.bottom + 25, window.width - 50, 100 } }
	, m_LoadedAmount{ 0 }
{
	m_LoadAmount = m_SpriteManager->LoadSize() + m_IntroScenes->LoadSize();
	m_LoadbarTick = m_LoadBar.width / m_LoadAmount;
}

ResourceLoader::~ResourceLoader()
{
	m_SpriteManager = nullptr;
	m_IntroScenes = nullptr;
}

void ResourceLoader::Load()
{
	if (!m_SpriteManager->IsDoneLoading())
	{
		m_SpriteManager->LoadTextures();
		++m_LoadedAmount;
	}
	else if (!m_IntroScenes->IsDoneLoading())
	{
		m_IntroScenes->LoadTextures();
		++m_LoadedAmount;
	}
	else
	{
		m_DoneLoading = true;
	}
}

void ResourceLoader::Draw() const
{
	utils::SetColor(Color4f{ 1, 1, 1, 1 });
	utils::FillRect(Rectf{m_LoadBar.left, m_LoadBar.bottom, m_LoadbarTick * (m_LoadedAmount - 1), m_LoadBar.height});
	utils::SetColor(Color4f{ 0, 0, 0, 1 });
	utils::DrawRect(m_LoadBar);
}

bool ResourceLoader::HasLoaded() const
{
	return m_DoneLoading;
}