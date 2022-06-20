#pragma once
#include "Texture.h"
#include "Timer.h"

class SaveFile final
{
public:
	SaveFile(Rectf window);
	~SaveFile();
	SaveFile(const SaveFile& other) = delete;
	SaveFile& operator=(const SaveFile& other) = delete;
	SaveFile(SaveFile&& other) = delete;
	SaveFile& operator=(SaveFile&& other) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	std::string GetName() const;

private:
	std::string m_Name;
	Rectf m_Window;

	std::string m_FontPath;
	Texture* m_Nametext;
	Texture* m_Text;


	Timer* m_KeyDelay;
	bool m_KeyIsPressed;

	Point2f getWindowCenter() const;
	void updateText();
};

