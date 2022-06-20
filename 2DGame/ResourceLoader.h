#pragma once
class SpriteManager;
class IntroSceneManager;

class ResourceLoader final
{
public:
	ResourceLoader(SpriteManager* spriteManager, IntroSceneManager* introSceneManager, Rectf window );
	~ResourceLoader();
	ResourceLoader(const ResourceLoader& other) = delete;
	ResourceLoader& operator=(const ResourceLoader& other) = delete;
	ResourceLoader(ResourceLoader&& other) = delete;
	ResourceLoader& operator=(ResourceLoader&& other) = delete;

	void Load();
	void Draw() const;

	bool HasLoaded() const;

private:
	Rectf m_Window;
	Rectf m_LoadBar;

	float m_LoadbarTick;
	int m_LoadedAmount;

	SpriteManager* m_SpriteManager;
	IntroSceneManager* m_IntroScenes;

	int m_LoadAmount;
	bool m_DoneLoading;

};

