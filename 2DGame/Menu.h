#pragma once
#include <vector>
#include "Button.h"
#include "Texture.h"
#include "Timer.h"

class Menu
{
public:
	enum class buttonBehaviour{ frozen, pulsing };
	enum class backGroundType{ texture, color };

	Menu( std::string texturePath, buttonBehaviour buttonBehaviour );
	Menu( Color4f BackGroundcolor, buttonBehaviour buttonBehaviour );

	virtual ~Menu();
	Menu(const Menu& other) = delete;
	Menu& operator=(const Menu& other) = delete;
	Menu(Menu&& other) = delete;
	Menu& operator=(Menu&& other) = delete;

	virtual void Update(float elapsedSec);
	virtual void Draw(Rectf window) const;

	void AddButton(std::string text, Rectf buttonRect, Color4f ButtonColor, Color4f textColor);
	void AddButton(std::string text, Button::HorizontalAlligment horizontal, float vertical, float width, float height, Color4f ButtonColor, Color4f textColor, Rectf window, int textSize);

	void ButtonOffset( Point2f offset );

	int getIndexActiveButton() const;
	std::string getTextActiveButton() const;

protected:
	std::vector<Button*> m_Buttons;

	int m_CurrentButton;

	backGroundType m_BackGroundType;

	Texture* m_BackgroundTexture;
	Color4f m_BackGroundColor;

	buttonBehaviour m_ButtonBehaviour;
	Timer* m_PulsTimer;

	bool m_IsButtonPressed;
	bool m_ShowButton;

	void ButtonSelector();
};

