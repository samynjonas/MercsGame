#pragma once
#include <vector>
#include "MouseButton.h"

class ScrollBox final
{
public:
	ScrollBox(Rectf scrollBoxRect, Color4f backGroundColor, Rectf window);
	~ScrollBox();
	ScrollBox(const ScrollBox& other) = delete;
	ScrollBox& operator=(const ScrollBox& other) = delete;
	ScrollBox(ScrollBox&& other) = delete;
	ScrollBox& operator=(ScrollBox&& other) = delete;

	void AddButton(std::string text, Color4f textColor, Color4f Buttoncolor, Rectf window);

	void Update();
	void Draw() const;

	int GetButtonInfo();
	std::string GetButtonText();

	bool IsAButtonPressed() const;

private:
	std::vector<MouseButton*> m_AvailableButtons;
	MouseButton* m_ScrolBar;

	std::string m_ActiveText;

	Rectf m_Window;
	Rectf m_ScrollBox;
	Color4f m_BackgroundColor;

	Point2f GetMousePos() const;

	void DeActivateAllButtons();
};

