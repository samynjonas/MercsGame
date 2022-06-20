#include "pch.h"
#include "IntroSceneManager.h"

IntroSceneManager::IntroSceneManager()
	: m_CurrentScene{ 0 }
	, m_TransitionScene{ new Transition{ 3.f } }
	, m_hasSwitched{ false }
	, m_LoadedScenes{ 0 }
	, m_AmountOfScenes{ 3 }
	, m_IsLoaded{ false }
{

}

IntroSceneManager::~IntroSceneManager()
{
	for (size_t index = 0; index < m_Scenes.size(); index++)
	{
		delete m_Scenes[index];
		m_Scenes[index] = nullptr;
	}
	m_Scenes.clear();

	delete m_TransitionScene;
	m_TransitionScene = nullptr;
}

void IntroSceneManager::Update(float elapsedSec)
{
	m_Scenes[m_CurrentScene]->Update(elapsedSec);

	if (m_hasSwitched == false)
	{
		if (m_Scenes[m_CurrentScene]->IsSceneDone())
		{
			if (m_CurrentScene < int(m_Scenes.size()) - 1)
			{
				m_TransitionScene->Update(elapsedSec);
				if (m_TransitionScene->HalfTransition())
				{
					++m_CurrentScene;
					m_hasSwitched = true;
				}
			}
		}
	}
	else
	{
		m_TransitionScene->Update(elapsedSec);
		if (m_TransitionScene->IsTransitionComplete())
		{
			m_TransitionScene->ResetTransition();
			m_hasSwitched = false;
		}
	}
}
void IntroSceneManager::Draw( Rectf window ) const
{
	m_Scenes[m_CurrentScene]->Draw(window);

	//Frame correction because I am lazy
	utils::SetColor(Color4f{ 0,0,0,1 });
	utils::FillRect(window.width - 200, 0, 200, 100);


	if (m_Scenes[m_CurrentScene]->IsSceneDone())
	{
		if (m_CurrentScene == 1)
		{
			m_TransitionScene->Draw(Color4f{ 1,0,0,1 }, window);
		}
		else
		{
			m_TransitionScene->Draw(Color4f{ 0,0,0,1 }, window);
		}
	}
}
bool IntroSceneManager::isDone() const
{
	if (m_CurrentScene == m_Scenes.size() - 1)
	{
		if (m_Scenes[m_CurrentScene]->IsSceneDone())
		{
			return true;
		}
	}
	return false;
}

void IntroSceneManager::LoadTextures()
{
	if (m_LoadedScenes < m_AmountOfScenes)
	{
		switch (m_LoadedScenes)
		{
		case 0:
			m_Scenes.push_back(new IntroScene{ "..\\Resources\\IntroScene\\MercsScene1\\", ".jpg", 45, 24 });
			break;
		case 1:
			m_Scenes.push_back(new IntroScene{ "..\\Resources\\IntroScene\\MercsScene2\\", ".jpg", 41, 24 });
			break;
		case 2:
			m_Scenes.push_back(new IntroScene{ "..\\Resources\\IntroScene\\MercsScene3\\", ".jpg", 55, 24 });
			break;
		}
		++m_LoadedScenes;
	}
	else
	{
		m_IsLoaded = true;
	}
}
int IntroSceneManager::LoadSize() const
{
	return m_AmountOfScenes;
}
bool IntroSceneManager::IsDoneLoading() const
{
	return m_IsLoaded;
}