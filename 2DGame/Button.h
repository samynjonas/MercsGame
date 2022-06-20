#pragma once
#include "Texture.h"

class Button final
{
public:
	enum class HorizontalAlligment { left, center, right };;

	Button( std::string text, Rectf buttonRect, Color4f ButtonColor, Color4f textColor, int textSize );
	Button( std::string text, HorizontalAlligment horizontal, float vertical, float width, float height, Color4f ButtonColor, Color4f textColor, Rectf window, int textSize );
	~Button();
	Button(const Button& other) = delete;
	Button& operator=(const Button& other) = delete;
	Button(Button&& other) = delete;
	Button& operator=(Button&& other) = delete;

	void Draw() const;

	void Enable();
	void Dissable();

	Rectf GetRect() const;
	void SetPosition( Point2f pos );

	bool isEnabled() const;
	std::string GetText() const;
	float getHorizontalPos(HorizontalAlligment& horizontal, Rectf window, int textSize, float width) const;

private:
	Texture* m_TextTexture;
	std::string m_FontPath;
	std::string m_Text;

	Rectf m_ButtonRect;
	Color4f m_ButtonColor;

	Color4f m_NonActiveColor;
	Color4f m_ActiveColor;


	bool m_IsActive;

	void UpdateColor();
};

