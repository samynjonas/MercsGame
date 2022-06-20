#include "pch.h"
#include "Debugger.h"
#include "Texture.h"

Debugger::Debugger(Rectf window)
	: m_IsEnabled{ false }
	, m_IsButtonPressed{ false }
	, m_FPS{ 0 }
	, m_FontPath{ "..\\Resources\\Fonts\\RetroFont.ttf" }
	, m_FPStexture{ nullptr }
	, m_Window{ window }
{
	updateText();
}

Debugger::~Debugger()
{
	delete m_FPStexture;
	m_FPStexture = nullptr;
}

void Debugger::Update(float elapsedSec)
{
	int prevFPS = m_FPS;
	m_FPS = int(1.f / elapsedSec);
	if ( abs(m_FPS - prevFPS) > 25 || m_FPS < 100)
	{
		updateText();
	}	

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_I] && m_IsButtonPressed == false)
	{
		m_IsEnabled = !m_IsEnabled;
		m_IsButtonPressed = true;
		printControls();
	}
	else if (!pStates[SDL_SCANCODE_I])
	{
		m_IsButtonPressed = false;
	}
}

void Debugger::Draw() const
{
	if (m_IsEnabled)
	{
		m_FPStexture->Draw(Point2f{25, m_Window.bottom + m_Window.height - 50 });
	}
}

void Debugger::updateText()
{
	delete m_FPStexture;
	m_FPStexture = new Texture{ std::to_string(m_FPS), m_FontPath, 25, Color4f{ 1, 0, 0, 1 } };
}
void Debugger::printControls() const
{
	if (m_IsEnabled)
	{
		std::cout << "ZQSD - WASD : moving player one" << std::endl;
		std::cout << "E :Shooting player one" << std::endl;
		std::cout << "R :Bomb throwing player one" << std::endl;
		std::cout << std::endl;
	}	
}

bool Debugger::IsEnabled() const
{
	return m_IsEnabled;
}