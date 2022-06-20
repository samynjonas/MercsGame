#include "pch.h"

#include "Level.h"
#include "Texture.h"
#include "SVGParser.h"

#include "BossClass.h"

#include "Actor.h"
#include "Player.h"

#include "Enemy.h"
#include "EnemyAssaultTrooper.h"
#include "EnemySpawner.h"

#include "Object.h"
#include "PowerUp.h"

#include "ArcadeMachine.h"
#include "HUD.h"
#include "Camera.h"

Level::Level(Rectf window, float scale, ResourceManager* resourceManager)
	: m_Window{ window }
	, m_Scale{ scale }
	, m_PlayerCount{ 1 }
	, m_MaxPlayers{ 2 }
	, m_pPlayerOne{}
	, m_pPlayerTwo{}
	, m_EnemyCount{ 0 }
	, m_HUD{ new HUD{} }
	, m_Score{ 0 }
	, m_ObjectScore{ 150 }
	, m_EnemyScore{ 50 }
	, m_BossScore{ 500 }
	, m_ResourceManager{ resourceManager }
	, m_Texture{ nullptr }
{
	InitializeHUD();
	AddActors();
}

void Level::InitializeHUD()
{
	m_Arcade = new ArcadeMachine{ m_Window };
}
void Level::ImportActor(Actor* actor)
{
	if (actor->getActorType() == Actor::actorType::enemy)
	{
		float spawnDelay{ 2.f };
		int maxSpawned{ 1 };
		float range{ 250.f };
		float despawnRange{ 3000.f };

		m_EnemySpawners.push_back( new EnemySpawner{ actor, spawnDelay, maxSpawned, range, despawnRange, m_Window, m_ResourceManager });
	}
	else
	{
		m_Actors.push_back(actor);
	}

	for (Actor* actor : m_Actors)
	{
		if (actor->getActorType() == Actor::actorType::player)
		{
			if (m_pPlayerOne == nullptr && m_PlayerCount >= 1)
			{
				m_pPlayerOne = static_cast<Player*>(actor);
			}
			else if (m_pPlayerTwo == nullptr && m_PlayerCount >= 2)
			{
				if (actor != m_pPlayerOne)
				{
					m_pPlayerTwo = static_cast<Player*>(actor);
				}
			}
		}
	}
}
void Level::LoadMap(std::map<std::string, std::string> mapPaths)
{
	for (auto& map : mapPaths)
	{
		m_Texture = new Texture{ map.first };
		SVGMapLoading(map.second);
	}
	InitializeCamera();
}

void Level::AddActors()
{
	//Boss
	m_LevelBoss = new BossClass{ Rectf{ 900, 1700, 500, 400 }, BossClass::BossType::Jet, m_Window, m_ResourceManager };
}
void Level::InitializeCamera()
{
	m_PlayerCamera = new Camera{ m_Window.width,  m_Window.height };
	m_PlayerCamera->SetLevelBoundaries(Rectf{ 50, 30, getMapRect().width, getMapRect().height});
}
 
Level::~Level()
{
	m_ResourceManager = nullptr;

	delete m_HUD;
	m_HUD = nullptr;

	delete m_Texture;
	m_Texture = nullptr;

	delete m_Arcade;
	m_Arcade = nullptr;

	delete m_LevelBoss;
	m_LevelBoss = nullptr;

	delete m_PlayerCamera;
	m_PlayerCamera = nullptr;
	
	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		delete m_Actors[index];
		m_Actors[index] = nullptr;
	}
	m_Actors.clear();

	m_pPlayerOne = nullptr;
	m_pPlayerTwo = nullptr;

	for (size_t index = 0; index < m_EnemySpawners.size(); index++)
	{
		m_EnemySpawners[index]->killAll();
		delete m_EnemySpawners[index];
		m_EnemySpawners[index] = nullptr;
	}
	m_EnemySpawners.clear();
}

void Level::Draw() const
{
	glPushMatrix();
		glTranslatef(m_Window.left, m_Window.bottom, 0);
		m_PlayerCamera->Transform(m_pPlayerOne->getActorRect());
		DrawMapTexture(m_Scale);
		m_LevelBoss->Draw();
		if (m_ResourceManager->Debug()->IsEnabled())
		{
			utils::DrawPolygon( m_Vertices[0], false, 10 );
		}
		for (Actor* actor : m_Actors)
		{
			if (actor != m_pPlayerOne)
			{
				if (m_pPlayerTwo == nullptr || actor != m_pPlayerTwo)
				{
					actor->Draw();
				}
			}
		}
		m_pPlayerOne->Draw();
		if (m_pPlayerTwo != nullptr)
		{
			m_pPlayerTwo->Draw();
		}
		for (size_t index = 0; index < m_EnemySpawners.size(); index++)
		{
			m_EnemySpawners[index]->Draw();
		}
	glPopMatrix();

	m_Arcade->Draw();
	m_HUD->Draw();
}
void Level::Update(float elapsedSec)
{
	m_HUD->Update(elapsedSec);
	UpdateHUD();
	m_LevelBoss->Update(elapsedSec, this, m_pPlayerOne, m_pPlayerTwo);

	if (!m_LevelBoss->isBossActive())
	{
		for (size_t index = 0; index < m_EnemySpawners.size(); index++)
		{
			m_EnemySpawners[index]->isInRange(Point2f{ m_pPlayerOne->getActorRect().left, m_pPlayerOne->getActorRect().bottom });
			m_EnemySpawners[index]->Update(elapsedSec);
			m_EnemySpawners[index]->EnemyUpdating(m_pPlayerOne, elapsedSec, this);
			m_Score += m_EnemySpawners[index]->getScore();
		}
	}
	else
	{
		for (size_t index = 0; index < m_EnemySpawners.size(); index++)
		{
			m_EnemySpawners[index]->killAll();
		}
	}

	for (Actor* actor : m_Actors)
	{
		//Check for player 2
		if (actor->getActorType() == Actor::actorType::player)
		{
			if (m_pPlayerTwo == nullptr && m_PlayerCount >= 2)
			{
				if (actor != m_pPlayerOne)
				{
					m_pPlayerTwo = static_cast<Player*>(actor);
				}
			}
		}
		else if (actor->getActorType() == Actor::actorType::enemy)
		{
			Enemy* pEnemy{ static_cast<Enemy*>(actor) };
			
			//player one
			pEnemy->Movement(m_pPlayerOne);
			pEnemy->BulletHitActor(m_pPlayerOne);

			m_pPlayerOne->CollidingWithEnemy(pEnemy);

			if (m_pPlayerOne->getActorRect().bottom > pEnemy->getActorRect().bottom + 300 || m_LevelBoss->isBossActive())
			{
				pEnemy->KillActor();
			}

			//player two
			if (m_pPlayerTwo != nullptr)
			{
				pEnemy->Movement(m_pPlayerTwo);
				pEnemy->BulletHitActor(m_pPlayerTwo);

				m_pPlayerTwo->CollidingWithEnemy(pEnemy);
			}
		}
		else if (actor->getActorType() == Actor::actorType::object)
		{
			Object* pObject{ static_cast<Object*>(actor) };
			
			//player One
			m_pPlayerOne->ObjectColliding(pObject);

			//player Two
			if (m_pPlayerTwo != nullptr)
			{
				m_pPlayerTwo->ObjectColliding(pObject);
			}
		}
		else if (actor->getActorType() == Actor::actorType::powerUp)
		{
			PowerUp* pPowerUp{ static_cast<PowerUp*>(actor) };

			//player one
			pPowerUp->CheckPlayerColliding(m_pPlayerOne);

			//player two
			if (m_pPlayerTwo != nullptr)
			{
				pPowerUp->CheckPlayerColliding(m_pPlayerTwo);
			}
		}

		//Update all actors
		actor->Update(elapsedSec, this);

		//If the current actor is not the player, check if the player has hit an enemy
		if (actor != m_pPlayerOne)
		{
			m_pPlayerOne->BulletHitActor(actor);
		}
		if (m_pPlayerTwo != nullptr)
		{
			if (actor != m_pPlayerTwo)
			{
				m_pPlayerTwo->BulletHitActor(actor);
			}
		}

		//Is the player isnt a nullptr ask for the input and check for a collision with the camera
		if (m_pPlayerOne != nullptr)
		{
			m_pPlayerOne->getInput(m_Arcade->getHorizontalP1(), m_Arcade->getVerticalP1(), this);
			m_pPlayerOne->CheckCameraCollision(m_PlayerCamera);
		}
		if (m_pPlayerTwo != nullptr)
		{
			m_pPlayerTwo->getInput(m_Arcade->getHorizontalP2(), m_Arcade->getVerticalP2(), this);
			m_pPlayerTwo->CheckCameraCollision(m_PlayerCamera);
		}

		if (m_LevelBoss->isActorInRoom(m_pPlayerOne))
		{
			m_PlayerCamera->SnapAndFreeze(m_LevelBoss->getMiddlePoint());
		}
	}

	//Check for actor Dead
	CheckForActorDeath();

	//Update camera
	m_PlayerCamera->SetLevelBoundaries( Rectf{ 190, m_PlayerCamera->getCameraView().bottom, getMapRect().width,  getMapRect().height });

	//Update HUD
	m_Arcade->update(elapsedSec);
}

void Level::CheckForActorDeath()
{
	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		if (m_Actors[index]->IsActorDead())
		{
			if (m_Actors[index]->getActorType() == Actor::actorType::object)
			{
				m_Score += m_ObjectScore;
				Object* pObject{ static_cast<Object*>(m_Actors[index]) };
				if (pObject->getObjectType() == Object::objectType::Crate)
				{
					switch (pObject->getCrateType())
					{
					case Object::CrateType::Power:
						m_Actors.push_back(new PowerUp{ Rectf{ pObject->getActorRect().left, pObject->getActorRect().bottom, 30, 30}, m_Window, PowerUp::PowerUpType::Power, m_ResourceManager });
						break;
					case Object::CrateType::Food:
						m_Actors.push_back(new PowerUp{ Rectf{ pObject->getActorRect().left, pObject->getActorRect().bottom, 30, 30}, m_Window, PowerUp::PowerUpType::Food, m_ResourceManager });
						break;
					case Object::CrateType::Health:
						m_Actors.push_back(new PowerUp{ Rectf{ pObject->getActorRect().left, pObject->getActorRect().bottom, 30, 30}, m_Window, PowerUp::PowerUpType::Health, m_ResourceManager });
						break;
					case Object::CrateType::Bomb:
						m_Actors.push_back(new PowerUp{ Rectf{ pObject->getActorRect().left, pObject->getActorRect().bottom, 30, 30}, m_Window, PowerUp::PowerUpType::Bomb, m_ResourceManager });
						break;
					}
				}
			}
			else if (m_Actors[index]->getActorType() == Actor::actorType::enemy)
			{
				--m_EnemyCount;
				m_Score += m_EnemyScore;
			}
			m_Actors[index] = m_Actors.back();
			m_Actors.pop_back();
		}
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& velocity, Direction direction, int leftOrRight) const 
{	
	Point2f leftAndBottomPoint;
	Point2f rightAndTopPoint;

	Point2f middlePoint{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	float offset{ 3 };
	if (direction == Direction::horizontal)
	{
		leftAndBottomPoint	= Point2f{ actorShape.left + offset,					middlePoint.y };
		rightAndTopPoint	= Point2f{ actorShape.left + actorShape.width - offset, middlePoint.y };
	}
	else if (direction == Direction::vertical)
	{
		leftAndBottomPoint	= Point2f{ middlePoint.x, actorShape.bottom + offset };
		rightAndTopPoint	= Point2f{ middlePoint.x, actorShape.bottom + actorShape.height - offset };
	}
	utils::HitInfo hitInfo;

	if (leftOrRight < 0) // left
	{
		if (direction == Direction::horizontal)
		{
			//Border left
			if (utils::Raycast(m_Vertices[0], middlePoint, leftAndBottomPoint, hitInfo))
			{
				velocity.x = 0;
				actorShape.left = hitInfo.intersectPoint.x;
			}
		}
		else
		{
			//Border bottom
			if (utils::Raycast(m_Vertices[0], middlePoint, leftAndBottomPoint, hitInfo))
			{
				velocity.y = 0;
				actorShape.bottom = hitInfo.intersectPoint.y;
			}
		}
	}
	else
	{
		if (direction == Direction::horizontal)
		{
			if (utils::Raycast(m_Vertices[0], middlePoint, rightAndTopPoint, hitInfo))
			{
				//border right
				velocity.x = 0;
				actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
			}
		}
		else
		{
			if (utils::Raycast(m_Vertices[0], middlePoint, rightAndTopPoint, hitInfo))
			{
				//top bottom
				velocity.y = 0;
				actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height;
			}
		}
	}
}
bool Level::IsTouchingBorder(const Rectf& actorShape, Direction direction, int leftOrRight) const
{
	Point2f leftAndBottomPoint;
	Point2f rightAndTopPoint;

	Point2f middlePoint{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	
	float offset{ -5 };

	if (direction == Direction::horizontal)
	{
		leftAndBottomPoint	= Point2f{ actorShape.left + offset, middlePoint.y };
		rightAndTopPoint	= Point2f{ actorShape.left + actorShape.width - offset, middlePoint.y };
	}
	else if (direction == Direction::vertical)
	{
		leftAndBottomPoint	= Point2f{ middlePoint.x, actorShape.bottom + offset };
		rightAndTopPoint	= Point2f{ middlePoint.x, actorShape.bottom + actorShape.height - offset };
	}
	utils::HitInfo hitInfo{};

	if (leftOrRight == 1) // check right or top
	{
		return utils::Raycast(m_Vertices[0], middlePoint, rightAndTopPoint, hitInfo);
	}
	return utils::Raycast(m_Vertices[0], middlePoint, leftAndBottomPoint, hitInfo);
}

void Level::DrawMapTexture(float scale) const
{
	Rectf destRect;
	destRect.left	= 50;
	destRect.bottom	= 30;
	destRect.width	= m_Texture->GetWidth()  * 0.5f;
	destRect.height = m_Texture->GetHeight() * 0.5f;

	m_Texture->Draw(destRect);
}

void Level::SVGMapLoading(std::string SVGfilePath)
{
	m_LevelSVG = new SVGParser();
	m_LevelSVG->GetVerticesFromSvgFile(SVGfilePath, m_Vertices);
	delete m_LevelSVG;
	m_LevelSVG = nullptr;

	//Replacing of the SVG map
	for (size_t index = 0; index < m_Vertices[0].size(); index++)
	{
		m_Vertices[0][index].x *= 0.5f;
		m_Vertices[0][index].y *= 0.5f;

		m_Vertices[0][index].x += 50.f;
		m_Vertices[0][index].y += 30.f;
	}
}

Rectf Level::getMapRect() const
{
	return Rectf{ 0, 0, m_Texture->GetWidth(), m_Texture->GetHeight() };
}

float Level::getLength(Point2f p1, Point2f p2)
{
	float length{};
	length = sqrt((p1.x + p2.x) * (p1.x + p2.x) + (p1.y + p2.y) * (p1.y + p2.y));
	return length;
}
void Level::UpdateHUD()
{
	m_HUD->SetMaxHealth(m_pPlayerOne->getMaxHealth());
	m_HUD->UpdateHealth(m_pPlayerOne->getHealth());
	m_HUD->UpdateBombs(m_pPlayerOne->getBombs());
	m_HUD->UpdateScore(m_Score);
}

void Level::Reset()
{
	if (m_ResourceManager->Debug()->IsEnabled())
	{
		std::cout << "Reset level" << std::endl;
	}

	delete m_Texture;
	m_Texture = nullptr;

	m_Vertices.clear();

	for (size_t index = 0; index < m_Actors.size(); index++)
	{
		delete m_Actors[index];
		m_Actors[index] = nullptr;
	}
	m_Actors.clear();

	m_pPlayerOne = nullptr;
	m_pPlayerTwo = nullptr;

	for (size_t index = 0; index < m_EnemySpawners.size(); index++)
	{
		m_EnemySpawners[index]->killAll();
		delete m_EnemySpawners[index];
		m_EnemySpawners[index] = nullptr;
	}
	m_EnemySpawners.clear();

	AddActors();
	m_HUD->reset();

	m_Score = 0;
	m_EnemyScore = 0;
	m_ObjectScore = 0;
	m_BossScore = 0;
}

bool Level::HasKilledBoss() const
{
	return m_LevelBoss->IsActorDead();
}
bool Level::isGameOver() const
{
	if (m_pPlayerOne->IsActorDead() || m_HUD->GetTime() <= 0)
	{
		return true;
	}
	return false;
}