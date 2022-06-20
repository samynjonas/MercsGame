#pragma once
class Texture;

class ArcadeMachine final
{
public:
	ArcadeMachine( Rectf screenSize );
	~ArcadeMachine();

	ArcadeMachine(const ArcadeMachine& other) = delete;
	ArcadeMachine& operator=(const ArcadeMachine& other) = delete;
	ArcadeMachine(ArcadeMachine&& other) = delete;
	ArcadeMachine& operator=(ArcadeMachine&& other) = delete;

	void Draw() const;
	void update(float elapsedSec);

	int getHorizontalP1() const;
	int getHorizontalP2() const;

	int getVerticalP1() const;
	int getVerticalP2() const;


private:
	Texture* m_Joysticktextures;

	Rectf m_TVScreen;

	const int m_AmountOfButtons{ 4 };
	Point2f* m_ButtonsPos;
	bool* m_ButtonPressed;

	const int m_AmountofJoyStick{ 2 };
	Rectf* m_JoyStickBox;

	enum class VerticalAxis		{ up = 1,	 middle = 0, down = -1 };
	enum class HorizontalAxis	{ left = -1, middle = 0, right = 1 };

	VerticalAxis* m_JoystickVertical;
	HorizontalAxis* m_JoyStickHorizontal;
	Point2f* m_JoystickBallPos;

	void DrawMachine() const;

};

