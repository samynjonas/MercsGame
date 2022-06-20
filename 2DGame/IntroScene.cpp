#include "pch.h"
#include "IntroScene.h"


IntroScene::IntroScene(std::string mapLocation, std::string fileType, int amountOfFrames, float framesPerSec)
	: m_Delay{ new Timer{} }
	, m_CurrentFrame{ 0 }
	, m_IsDone{ false }
{
	m_Delay->setTimeGoal(float(1 / framesPerSec));

	for (int index = 0; index < amountOfFrames; index++)
	{
		m_SceneFrames.push_back(new Texture{ mapLocation + std::to_string(index) + fileType });
	}
}

IntroScene::~IntroScene()
{
	delete m_Delay;
	m_Delay = nullptr;

	for (size_t index = 0; index < m_SceneFrames.size(); index++)
	{
		delete m_SceneFrames[index];
		m_SceneFrames[index] = nullptr;
	}
	m_SceneFrames.clear();
}

void IntroScene::Update(float elapsedSec)
{
	m_Delay->Update(elapsedSec);
	if (m_Delay->isTimerDone())
	{
		if(m_CurrentFrame < int(m_SceneFrames.size()) - 1)
		{
			++m_CurrentFrame;
		}
		else
		{
			m_IsDone = true;
		}
		m_Delay->resetTimer();
	}
}

void IntroScene::Draw(Rectf window) const
{
	m_SceneFrames[m_CurrentFrame]->Draw(window);
}

bool IntroScene::IsSceneDone() const
{
	return m_IsDone;
}