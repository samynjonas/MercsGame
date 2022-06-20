#pragma once
#include "Button.h"

class PauseMenu final
{
public:
	PauseMenu( Rectf pauseRect, Color4f backgroundColor );
	~PauseMenu();
	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) = delete;

	void Update();
	void Draw() const;

	void AddButton( Button* button );

	void DeActivateAll();

	bool ContinueGame() const;
	bool QuitToGame() const;

private:
	std::vector<Button*> m_Buttons;

	Rectf m_PauseRect;
	Color4f m_BackgroundColor;

	bool m_IsButtonPressed;
	int m_CurrentButton;	

	bool m_IsContinuePressed;
	bool m_IsQuitToGamePressed;

	void ButtonSelector();
	void CheckForButtonPress();
};

