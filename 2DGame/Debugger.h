#pragma once
class Debugger final
{
public:
	Debugger(Rectf window);
	~Debugger();
	Debugger(const Debugger& other) = delete;
	Debugger& operator=(const Debugger& other) = delete;
	Debugger(Debugger&& other) = delete;
	Debugger& operator=(Debugger&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void printControls() const;

	bool IsEnabled() const;

private:
	bool m_IsEnabled;
	bool m_IsButtonPressed;

	int m_FPS;
	Texture* m_FPStexture;
	std::string m_FontPath;

	Rectf m_Window;

	void updateText();
};

