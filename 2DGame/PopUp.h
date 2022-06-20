#pragma once
#include "MouseButton.h"
#include "Texture.h"

class PopUp final
{
public:
	PopUp( Rectf rect, std::string popUpText, Color4f color, Rectf window);
	~PopUp();
	PopUp(const PopUp& other) = delete;
	PopUp& operator=(const PopUp& other) = delete;
	PopUp(PopUp&& other) = delete;
	PopUp& operator=(PopUp&& other) = delete;

	void Draw() const;
	
	bool IsOkPressed() const;
	bool IsActive() const;

	void Pop();
	void Close();


private:
	Rectf m_PopUpRect;
	Rectf m_Window;

	MouseButton* m_OkButton;
	Texture* m_PopUpText;
	Color4f m_Color;

	bool m_Show;

	Point2f Center() const;
};

