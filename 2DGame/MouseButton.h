#pragma once
#include "Texture.h"
#include "Sprite.h"

class MouseButton final
{
public:
	MouseButton(Rectf buttonRect, std::string text, Color4f textColor, int textSize, Color4f Buttoncolor, Rectf window, int info);
	MouseButton(Rectf buttonRect, Texture* texture, Rectf window, int info);
	MouseButton(Rectf buttonRect, SpriteInfo* spriteInfo, Rectf window, int info);
	~MouseButton();
	MouseButton(const MouseButton& other) = delete;
	MouseButton& operator=(const MouseButton& other) = delete;
	MouseButton(MouseButton&& other) = delete;
	MouseButton& operator=(MouseButton&& other) = delete;

	void Draw(bool showBackground) const;

	bool IsButtonPressed();
	bool IsButtonReleased();
	bool IsHoldingMouse();

	int GetButtonInfo() const;

	Rectf GetRect() const;
	void SetPosition(Point2f pos);

	void SetHeight( float height );
	void SetWidth(float width);

	std::string GetButtonText() const;

	void Activate();
	void DeActivate();

	void UpdateSprite();

private:
	std::string m_Text;

	Rectf m_Window;
	Rectf m_ButtonRect;

	Color4f m_Color;
	Texture* m_Texture;

	Sprite* m_Sprite;
	SpriteInfo* m_SpriteInfo;

	bool m_IsPressed;
	bool m_IsHolding;
	
	bool m_MouseOnButton;

	int m_InformationInt;

	bool m_IsButtonActive;

	Point2f getMousePos() const;
	bool LeftMouse() const;
	bool RightMouse() const;
};

