#include "pch.h"
#include "LevelEditor.h"
#include "Object.h"
#include "Enemy.h"
#include "EnemyAssaultTrooper.h"

#include <vector>

LevelEditor::LevelEditor(Rectf window, ResourceManager* resourceManager)
	: m_Camera{ new Camera{window.width, window.height}}
	, m_FollowPoint{ window.width, window.height, 10, 10 }
	, m_FileWriter{ new FileWriter{"..\\Resources\\customMaps"} }
	, m_RemovedObject{ false }
	, m_Window{ window }
	, m_SaveButton{ new MouseButton{ Rectf{ window.left + window.width - 150, 25, 100, 25}, "SAVE", Color4f{1, 1, 1, 1}, 30,  Color4f{1,0,0,1}, window, 0} }
	, m_TabLeft{  new MouseButton{ Rectf{ 50 - 50, window.height - 50, 25, 25}, "<--", Color4f{0, 0, 0, 1}, 30,  Color4f{0.8f, 0.8f, 0.8f, 1}, window, 0} }
	, m_TabRight{ new MouseButton{ Rectf{ 50 + 25, window.height - 50, 25, 25}, "-->", Color4f{0, 0, 0, 1}, 30,  Color4f{0.8f, 0.8f, 0.8f, 1}, window, 0} }
	, m_Saver{ new SaveFile{ window } }
	, m_Saving{ false }
	, m_HasPlacedPlayer{ false }
	, m_Moving{ false }
	, m_PopUp{ new PopUp{ Rectf{ window.width / 2 - 75, window.height / 2 - 50, 150, 100 }, "ADD PLAYER", Color4f{0.25f, 0.25f, 0.25f, 1}, window } }
	, m_ResourceManager{ resourceManager }
	, m_SaveFileName{ " " }
	, m_Debug{ false }
	, m_MapsFolder{ new ReadFolder{} }
	, m_SVGFolder{ new ReadFolder{} }
	, m_CurrentMap{ 0 }
	, m_MapOffset{ Point2f{ 50, 125 } }
	, m_MapScale{ 0.5f }
	, m_LevelSVG{ nullptr }
{
	m_MapsFolder->SetFolderPath("..\\Resources\\Maps\\PNG");
	m_SVGFolder->SetFolderPath( "..\\Resources\\Maps\\SVG");
	for (auto& maps : m_MapsFolder->GetFolderContent())
	{
		m_Maps.push_back(maps.first);
	}
	m_MapSelector = new OptionSelector{ m_Maps, 10, Color4f{1, 1, 0, 1}, OptionSelector::HorizontalPlacement::center, OptionSelector::VerticalPlacement::top, window, resourceManager };

	m_MapTexture = new Texture{ m_MapsFolder->GetFolderContent()[m_Maps[m_CurrentMap]] };
	SVGMapLoading(m_SVGFolder->GetFolderContent()[m_Maps[m_CurrentMap]]);

	AddButtons();

	m_Camera->SetLevelBoundaries(window);
}

LevelEditor::~LevelEditor()
{
	m_ResourceManager = nullptr;

	delete m_MapTexture;
	m_MapTexture = nullptr;

	delete m_MapsFolder;
	m_MapsFolder = nullptr;

	delete m_SVGFolder;
	m_SVGFolder = nullptr;

	delete m_LevelSVG;
	m_LevelSVG = nullptr;

	delete m_MapSelector;
	m_MapSelector = nullptr;

	delete m_Camera;
	m_Camera = nullptr;

	delete m_FileWriter;
	m_FileWriter = nullptr;

	delete m_SaveButton;
	m_SaveButton = nullptr;

	delete m_TabLeft;
	m_TabLeft = nullptr;

	delete m_TabRight;
	m_TabRight = nullptr;

	delete m_Saver;
	m_Saver = nullptr;

	delete m_PopUp;
	m_PopUp = nullptr;

	for (size_t index = 0; index < m_ObjectButtons.size(); index++)
	{
		delete m_ObjectButtons[index];
		m_ObjectButtons[index] = nullptr;
	}
	m_ObjectButtons.clear();

	for (size_t index = 0; index < m_SpawnerButtons.size(); index++)
	{
		delete m_SpawnerButtons[index];
		m_SpawnerButtons[index] = nullptr;
	}
	m_SpawnerButtons.clear();

	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		delete m_Actors[index];
		m_Actors[index] = nullptr;
	}
	m_Actors.clear();
}

void LevelEditor::Update(float elapsedSec)
{
	if (m_PopUp->IsActive() == false)
	{
		if (m_Saving == false)
		{
			const Uint8* pStates = SDL_GetKeyboardState(nullptr);
			if (pStates[SDL_SCANCODE_Q] || pStates[SDL_SCANCODE_A])
			{
				//Move to Left
				--m_FollowPoint.left;
			}
			if (pStates[SDL_SCANCODE_D])
			{
				//Move to right
				++m_FollowPoint.left;
			}
			if (pStates[SDL_SCANCODE_Z] || pStates[SDL_SCANCODE_W])
			{
				//Move to up
				++m_FollowPoint.bottom;
			}
			if (pStates[SDL_SCANCODE_S])
			{
				//Move to down
				--m_FollowPoint.bottom;
			}
			m_Camera->SetLevelBoundaries(Rectf{ m_FollowPoint.left - m_Window.width / 2, m_FollowPoint.bottom - m_Window.height / 2, m_Window.width, m_Window.height });

			CheckForRemoveActor();
			CheckForNewActor();

			m_MapSelector->Update();
			if (m_Maps[m_CurrentMap] != m_MapSelector->getCurrentText())
			{
				//Update map
				m_CurrentMap = m_MapSelector->getCurrentIndex();
				//std::cout << "Update map" << std::endl;
				m_MapTexture = new Texture{ m_MapsFolder->GetFolderContent()[m_Maps[m_CurrentMap]] };
				SVGMapLoading(m_SVGFolder->GetFolderContent()[m_Maps[m_CurrentMap]]);				
			}
		}
		else
		{
			m_Saver->Update(elapsedSec);

			if (m_SaveFileName == " ")
			{
				const Uint8* pStates = SDL_GetKeyboardState(nullptr);
				if (pStates[SDL_SCANCODE_RETURN])
				{
					m_FileWriter->Write(m_Actors, m_Saver->GetName(), m_Maps[m_CurrentMap]);
					m_Saving = false;
				}
			}
			else
			{
				m_FileWriter->Write(m_Actors, m_SaveFileName, m_Maps[m_CurrentMap]);
				m_Saving = false;
			}			
		}
		if (m_SaveButton->IsButtonPressed())
		{
			if (m_HasPlacedPlayer)
			{
				m_Saving = true;
			}
			else
			{
				m_PopUp->Pop();
			}
		}
		if (m_TabLeft->IsButtonPressed())
		{
			PreviousButtonTab();
		}
		if (m_TabRight->IsButtonPressed())
		{
			NextButtonTab();
		}
	}
	else
	{
		if (m_PopUp->IsOkPressed())
		{
			m_PopUp->Close();
		}
	}
}

void LevelEditor::DrawMapTexture() const
{
	Rectf destRect;
	destRect.left	= m_MapOffset.x;
	destRect.bottom = 0;
	destRect.width	= m_MapTexture->GetWidth() * m_MapScale;
	destRect.height = m_MapTexture->GetHeight() * m_MapScale;

	m_MapTexture->Draw(destRect);
}
void LevelEditor::SVGMapLoading(std::string SVGfilePath)
{
	m_Vertices.clear();

	m_LevelSVG = new SVGParser();
	m_LevelSVG->GetVerticesFromSvgFile(SVGfilePath, m_Vertices);
	delete m_LevelSVG;
	m_LevelSVG = nullptr;

	//Replacing of the SVG map
	for (size_t index = 0; index < m_Vertices[0].size(); index++)
	{
		m_Vertices[0][index].x *= m_MapScale;
		m_Vertices[0][index].y *= m_MapScale;

		m_Vertices[0][index].x += m_MapOffset.x;
		m_Vertices[0][index].y += 0;
	}
}

void LevelEditor::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Window.left, m_Window.bottom, 0);
		m_Camera->Transform(m_FollowPoint);
		DrawMapTexture();
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawPolygon(m_Vertices[0], false, 10);
		for (const Actor* actor : m_Actors)
		{
			actor->Draw();
		}
	glPopMatrix();

	utils::SetColor(Color4f{ 0.25f, 0.25f, 0.25f, 0.75f });
	utils::FillRect(0, 0, 100, m_Window.height);

	if (m_CurrentButtonTab == ButtonTab::objects)
	{
		for (MouseButton* button : m_ObjectButtons)
		{
			button->Draw(true);
		}
	}
	else if (m_CurrentButtonTab == ButtonTab::spawners)
	{
		for (MouseButton* button : m_SpawnerButtons)
		{
			button->Draw(true);
		}
	}

	m_TabLeft->Draw(true);
	m_TabRight->Draw(true);
	m_MapSelector->Draw(false);

	m_SaveButton->Draw(true);
	if (m_Saving == true)
	{
		utils::SetColor(Color4f{ 0.9f, 0.9f, 0.9f, 0.5f });
		utils::FillRect( m_Window );
		m_Saver->Draw();
	}
	m_PopUp->Draw();
}

Point2f LevelEditor::getMousePos() const
{
	int x, y;
	Uint32 mouse = { SDL_GetMouseState(&x,&y) };
	return Point2f{ float(x), float(m_Window.height - y) };
}

void LevelEditor::CheckForRemoveActor()
{
	int x, y;
	Uint32 mouse;
	mouse = SDL_GetMouseState(&x, &y);

	for (int index{ int(m_Actors.size()) - 1 }; index >= 0; index--)
	{
		if (m_RemovedObject == false)
		{
			float x{ getMousePos().x + m_Camera->getCameraView().left * 2 };
			float y{ getMousePos().y + m_Camera->getCameraView().bottom * 2 };

			if (utils::IsPointInRect(Point2f(x, y), Rectf{ m_Actors[index]->getActorRect().left + m_Camera->getCameraView().left ,m_Actors[index]->getActorRect().bottom + m_Camera->getCameraView().bottom, m_Actors[index]->getActorRect().width, m_Actors[index]->getActorRect().height }))
			{
				if ((mouse & SDL_BUTTON_RMASK) != 0)
				{
					if (m_Actors[index] != m_Actors.back())
					{
						m_Actors[index] = m_Actors.back();
					}
					m_Actors.pop_back();
					m_RemovedObject = true;
					CheckForPlayer();
				}
			}
		}		
	}

	if ((mouse & SDL_BUTTON_RMASK) == 0)
	{
		m_RemovedObject = false;
	}
}
void LevelEditor::CheckForNewActor()
{
	if (m_CurrentButtonTab == ButtonTab::objects)
	{
		for (MouseButton* button : m_ObjectButtons)
		{
			if (button->IsButtonPressed())
			{
				switch (button->GetButtonInfo())
				{
				case 0:
					m_Actors.push_back({ new Object{ Rectf{ 0, 0, 10, 150 }, m_Window, m_ResourceManager, Object::objectType::Tree } });
					break;
				case 1:
					m_Actors.push_back({ new Object{ Rectf{ 0, 0, 200, 200 }, m_Window, m_ResourceManager, Object::objectType::House} });
					break;
				case 2:
					m_Actors.push_back({ new Object{ Rectf{ 0, 0, 150, 40 }, m_Window, m_ResourceManager, Object::objectType::SandbagWall} });
					break;
				case 3:
					m_Actors.push_back({ new Object{ Rectf{ 0, 0, 50, 50 },	m_Window, m_ResourceManager, Object::objectType::Crate, Object::CrateType::Power } });
					break;
				}
			}
			if (button->IsHoldingMouse())
			{
				float x{ getMousePos().x + m_Camera->getCameraView().left };
				float y{ getMousePos().y + m_Camera->getCameraView().bottom };

				m_Actors.back()->SetPosition(Point2f{ x, y });
			}
		}
	}
	else if (m_CurrentButtonTab == ButtonTab::spawners)
	{
		bool placedSpawner{ true };

		for (MouseButton* button : m_SpawnerButtons)
		{
			if (button->IsButtonPressed())
			{
				switch (button->GetButtonInfo())
				{
				case 0:
					m_Actors.push_back( new EnemyAssaultTrooper{ Rectf{ 250, 80, 30, 40 }, m_Window, m_ResourceManager });
					break;
				case 1:
					CheckForPlayer();
					if (m_HasPlacedPlayer == false)
					{
						m_Actors.push_back({ new Player{ Rectf{ 0, 0, 30, 40}, m_Window, m_ResourceManager } });
						m_HasPlacedPlayer = true;
					}
					else
					{
						placedSpawner = false;
					}

					break;
				}
			}
			if (button->IsHoldingMouse())
			{
				if (button->GetButtonInfo() == 1)
				{
					m_Moving = true;
					if (placedSpawner == true)
					{
						if (m_Actors.back()->getActorType() == Actor::actorType::player)
						{
							float x{ getMousePos().x + m_Camera->getCameraView().left };
							float y{ getMousePos().y + m_Camera->getCameraView().bottom };

							m_Actors.back()->SetPosition(Point2f{ x, y });
						}
					}
				}
				else
				{
					float x{ getMousePos().x + m_Camera->getCameraView().left };
					float y{ getMousePos().y + m_Camera->getCameraView().bottom };

					m_Actors.back()->SetPosition(Point2f{ x, y });
				}				
			}
		}
	}	
}

void LevelEditor::saveFileName(std::string name)
{
	m_SaveFileName = name;
}
void LevelEditor::AddActor(Actor* actor)
{
	m_Actors.push_back(actor);
	if (actor->getActorType() == Actor::actorType::player)
	{
		m_HasPlacedPlayer = true;
		if (m_ResourceManager->Debug()->IsEnabled())
		{
			std::cout << "there is a player" << std::endl;
		}
	}
}

void LevelEditor::ReloadButtons()
{
	m_ObjectButtons[0]->UpdateSprite();
	m_ObjectButtons[1]->UpdateSprite();
	m_ObjectButtons[2]->UpdateSprite();
	m_ObjectButtons[3]->UpdateSprite();

	m_SpawnerButtons[0]->UpdateSprite();
	m_SpawnerButtons[1]->UpdateSprite();
}

void LevelEditor::ResetLevelEditor()
{
	m_SaveFileName = " ";
	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		delete m_Actors[index];
		m_Actors[index] = nullptr;
	}
	m_Actors.clear();	
}

void LevelEditor::NextButtonTab()
{
	int temp{ int(m_CurrentButtonTab) };
	++temp;
	if (temp >= m_AmountOfTabs)
	{
		temp = 0;
	}
	m_CurrentButtonTab = static_cast<ButtonTab>(temp);
}
void LevelEditor::PreviousButtonTab()
{
	int temp{ int(m_CurrentButtonTab) };
	--temp;
	if (temp < 0)
	{
		temp = (m_AmountOfTabs - 1);
	}
	m_CurrentButtonTab = static_cast<ButtonTab>(temp);
}

void LevelEditor::CheckForPlayer()
{
	m_HasPlacedPlayer = false;
	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		if (m_Actors[index]->getActorType() == Actor::actorType::player)
		{
			m_HasPlacedPlayer = true;
			if (m_Debug)
			{
				std::cout << "there is a player" << std::endl;
			}
		}
	}
}

void LevelEditor::AddButtons()
{
	m_ObjectButtons.push_back(new MouseButton{ Rectf{20, 275, 60, 100}, m_ResourceManager->Sprites()->GetSprite("Tree"), m_Window, 0 });
	m_ObjectButtons.push_back(new MouseButton{ Rectf{15, 175, 75, 80},  m_ResourceManager->Sprites()->GetSprite("House"), m_Window, 1 });
	m_ObjectButtons.push_back(new MouseButton{ Rectf{15, 100, 75, 35},  m_ResourceManager->Sprites()->GetSprite("Sandbag"), m_Window, 2 });
	m_ObjectButtons.push_back(new MouseButton{ Rectf{25, 25,  50, 50},  m_ResourceManager->Sprites()->GetSprite("Crates"), m_Window, 3 });

	m_SpawnerButtons.push_back(new MouseButton{ Rectf{20, 200, 60, 100}, m_ResourceManager->Sprites()->GetSprite("EnemyIdle"), m_Window, 0 });
	m_SpawnerButtons.push_back(new MouseButton{ Rectf{20, 300, 60, 100}, m_ResourceManager->Sprites()->GetSprite("PlayerIdle"), m_Window, 1 });
}