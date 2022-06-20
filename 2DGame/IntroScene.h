#pragma once
#include <vector>
#include "Timer.h"
#include "Texture.h"

class IntroScene final
{
public:
	IntroScene( std::string mapLocation, std::string fileType, int amountOfFrames, float framesPerSec);
	~IntroScene();
	IntroScene(const IntroScene& other) = delete;
	IntroScene& operator=(const IntroScene& other) = delete;
	IntroScene(IntroScene&& other) = delete;
	IntroScene& operator=(IntroScene&& other) = delete;

	void Update(float elapsedSec);
	void Draw(Rectf window) const;

	bool IsSceneDone() const;

private:
	std::vector<Texture*> m_SceneFrames;
	Timer* m_Delay;

	int m_CurrentFrame;

	bool m_IsDone;
};

