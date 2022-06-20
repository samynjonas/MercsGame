#pragma once
#include "IntroScene.h"
#include "Transition.h"

class IntroSceneManager final
{
public:
	IntroSceneManager();
	~IntroSceneManager();
	IntroSceneManager(const IntroSceneManager& other) = delete;
	IntroSceneManager& operator=(const IntroSceneManager& other) = delete;
	IntroSceneManager(IntroSceneManager&& other) = delete;
	IntroSceneManager& operator=(IntroSceneManager&& other) = delete;

	void Update(float elapsedSec);
	void Draw(Rectf window) const;

	bool isDone() const;

	bool IsDoneLoading() const;
	int LoadSize() const;
	void LoadTextures();

private:
	std::vector<IntroScene*> m_Scenes;

	Transition* m_TransitionScene;

	int m_LoadedScenes;
	int m_AmountOfScenes;
	bool m_IsLoaded;

	int m_CurrentScene;
	bool m_hasSwitched;


};

