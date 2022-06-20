#include "pch.h"
#include "Player.h"
#include "Object.h"

#include "Level.h"
#include "Sprite.h"
#include "Camera.h"
#include "Texture.h"
#include "BulletManager.h"

Player::Player(Rectf playerRect, Rectf window, ResourceManager* resourceManager)
	: m_WalkSpeed{ 150 }
	, Actor{ playerRect, window, 150, resourceManager }
	, m_MaxHealth{ 15 } 
	, m_IsHit{ false }
	, m_DrawHit{ false }
	, ThrowingBomb{ false }
	, m_Timer{ new Timer{} }
	, m_Delay{ new Timer{} }
	, m_Opacity{ 0.5f }
	, m_Bombs{ 3 }
	, m_ResourceManager{ resourceManager }
{
	m_Health = m_MaxHealth;

	m_ActionState = ActionState::moving;
	m_ActorType = actorType::player;

	delete m_pBullets;
	m_pBullets = new BulletManager{ BulletManager::BulletType::blueBullets, resourceManager };

	m_IdleSpriteInfo	= m_ResourceManager->Sprites()->GetSprite("PlayerIdle");
	m_WalkingSpriteInfo	= m_ResourceManager->Sprites()->GetSprite("PlayerWalking");
	m_DyingSpriteInfo	= m_ResourceManager->Sprites()->GetSprite("PlayerDying");
	
	delete m_IdleSprite;
	m_IdleSprite		= new Sprite{ m_IdleSpriteInfo->texture, m_IdleSpriteInfo->collums, m_IdleSpriteInfo->rows, m_IdleSpriteInfo->fps, m_IdleSpriteInfo->scale };
	
	m_WalkingSprite		= new Sprite{ m_WalkingSpriteInfo->texture, m_WalkingSpriteInfo->collums, m_WalkingSpriteInfo->rows, m_WalkingSpriteInfo->fps, m_WalkingSpriteInfo->scale };
	m_DyingSprite		= new Sprite{ m_DyingSpriteInfo->texture, m_DyingSpriteInfo->collums, m_DyingSpriteInfo->rows, m_DyingSpriteInfo->fps, m_DyingSpriteInfo->scale };
}

Player::~Player()
{
	delete m_Timer;
	m_Timer = nullptr;
	
	delete m_Delay;
	m_Delay = nullptr;

	m_ResourceManager = nullptr;

	m_WalkingSpriteInfo = nullptr;
	m_DyingSpriteInfo = nullptr;

	delete m_WalkingSprite;
	m_WalkingSprite = nullptr;

	delete m_DyingSpriteInfo;
	m_DyingSpriteInfo = nullptr;
}

void Player::Update(float elapsedSec, const Level* level)
{
	if (m_ActionState == ActionState::moving)
	{
		//Position updating
		CalculateSpeed();
		m_WalkingSprite->Update(elapsedSec, Sprite::CyleType::Cycle);

		m_ActorRect.left   += m_Velocity.x * elapsedSec;
		m_ActorRect.bottom += m_Velocity.y * elapsedSec;

		//Level collision
		level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::horizontal, int(m_Velocity.x / abs(m_Velocity.x)));
		level->HandleCollision(m_ActorRect, m_Velocity, Level::Direction::vertical,   int(m_Velocity.y / abs(m_Velocity.y)));
	}
	else if (m_ActionState == ActionState::idle)
	{
		m_IdleSprite->Update(elapsedSec, Sprite::CyleType::Cycle);
	}
	else if (m_ActionState == ActionState::dying)
	{
		m_pBullets->Update(elapsedSec);
		m_DyingSprite->Update(elapsedSec, Sprite::CyleType::SingleCycle);
		if (m_DyingSprite->HasDoneCycle())
		{
			m_Timer->Update(elapsedSec);
			if (m_Timer->isTimerDone())
			{
				m_IsActorDead = true;
			}
		}
		else
		{
			m_Timer->setTimeGoal(2);
		}
	}

	if (m_ActionState != ActionState::dying)
	{
		if (m_Velocity.x != 0 || m_Velocity.y != 0)
		{
			m_ActionState = ActionState::moving;
		}
		else
		{
			m_ActionState = ActionState::idle;
		}
		if (m_Velocity.x == 0)
		{
			//Vertical movement
			if (m_Velocity.y > 0)
			{
				//Going up
				m_IdleSprite->SwitchToRow(6);
				m_WalkingSprite->SwitchToRow(6);
			}
			else if (m_Velocity.y < 0)
			{
				//Going down
				m_IdleSprite->SwitchToRow(2);
				m_WalkingSprite->SwitchToRow(2);
			}
		}
		else if (m_Velocity.x > 0)
		{
			//going right
			if (m_Velocity.y > 0)
			{
				//right up
				m_IdleSprite->SwitchToRow(-1);
				m_WalkingSprite->SwitchToRow(-1);
			}
			else if (m_Velocity.y < 0)
			{
			//right down
				m_IdleSprite->SwitchToRow(1);
				m_WalkingSprite->SwitchToRow(1);
			}
			else if (m_Velocity.y == 0)
			{
				//straight right
				m_IdleSprite->SwitchToRow(0);
				m_WalkingSprite->SwitchToRow(0);
			}
		}
		else if (m_Velocity.x < 0)
		{
			//going left
			if (m_Velocity.y > 0)
			{
				//left up
				m_IdleSprite->SwitchToRow(5);
				m_WalkingSprite->SwitchToRow(5);
			}
			else if (m_Velocity.y < 0)
			{
				//left down
				m_IdleSprite->SwitchToRow(3);
				m_WalkingSprite->SwitchToRow(3);
			}
			else if (m_Velocity.y == 0)
			{
				//straight left
				m_IdleSprite->SwitchToRow(4);
				m_WalkingSprite->SwitchToRow(4);
			}
		}
		//Bullets and shooting
		m_pBullets->Update(elapsedSec);
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_E])
		{
			m_pBullets->Shoot(Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 }, m_Direction, 3, Point2f{0, 0});
		}
	
		if (pStates[SDL_SCANCODE_R] && ThrowingBomb == false)
		{
			if (m_Bombs > 0)
			{
				//std::cout << "Throw Bomb" << std::endl;
				m_pBullets->AddBullet(BulletManager::BulletType::bigBomb, Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 }, m_Direction, Point2f{ 0, 0 });
				ThrowingBomb = true;
				--m_Bombs;
				m_ActionState = ActionState::idle;
			}			
		}
		else if (!pStates[SDL_SCANCODE_R] && ThrowingBomb == true)
		{
			ThrowingBomb = false;
		}

		if (m_IsHit)
		{
			m_Timer->Update(elapsedSec);
			if (m_Timer->isTimerDone())
			{
				m_IsHit = false;
				m_Timer->resetTimer();
			}
		}
	}
}

void Player::Draw() const
{
	m_pBullets->Draw();
	switch (m_ActionState)
	{
	case ActionState::idle:
		m_IdleSprite->Draw(getActorRect(), m_IdleSpriteInfo->sourceRect);
		break;
	case ActionState::moving:
		m_WalkingSprite->Draw(getActorRect(), m_WalkingSpriteInfo->sourceRect);
		break;
	case ActionState::dying:
		m_DyingSprite->Draw(getActorRect(), m_DyingSpriteInfo->sourceRect);
		break;
	}

	if (m_ResourceManager->Debug()->IsEnabled())
	{
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_ActorRect);
	}
}

void Player::ObjectColliding(const Object* object)
{
	HorizontalColliding(object);
	VerticalColliding(object);
}

void Player::getInput(int horizontal, int vertical, const Level* level)
{
	if (horizontal != 0)
	{
		m_Direction.x = float(horizontal);
		m_Direction.y = float(vertical);
		if (!level->IsTouchingBorder(m_ActorRect, Level::Direction::horizontal, horizontal))
		{
			m_Velocity.x = m_WalkSpeed * horizontal;
		}
	}
	else
	{
		m_Velocity.x = 0;
	}

	if (vertical != 0)
	{
		m_Direction.x = float(horizontal);
		m_Direction.y = float(vertical);
		if (!level->IsTouchingBorder(m_ActorRect, Level::Direction::vertical, vertical))
		{
			m_Velocity.y = m_WalkSpeed * vertical;
		}
	}
	else
	{
		m_Velocity.y = 0;
	}
}
void Player::CalculateSpeed()
{
	float speedCheck{ sqrtf((m_Velocity.x * m_Velocity.x) + (m_Velocity.y * m_Velocity.y))};

	if (abs(speedCheck != m_WalkSpeed) && speedCheck != 0)
	{
		m_Velocity.x = m_Velocity.x / speedCheck * m_WalkSpeed;
		m_Velocity.y = m_Velocity.y / speedCheck * m_WalkSpeed;
	}
}
void Player::CheckCameraCollision( const Camera* camera )
{
	if (m_ActorRect.left < camera->getCameraView().left)
	{
		m_ActorRect.left = camera->getCameraView().left;
	}
	else if (m_ActorRect.left + m_ActorRect.width > camera->getCameraView().left + camera->getCameraView().width)
	{
		m_ActorRect.left = camera->getCameraView().left + camera->getCameraView().width - m_ActorRect.width;
	}
	if (m_ActorRect.bottom < camera->getCameraView().bottom)
	{
		m_ActorRect.bottom = camera->getCameraView().bottom;
	}
	else if (m_ActorRect.bottom + m_ActorRect.height > camera->getCameraView().bottom + camera->getCameraView().height)
	{
		m_ActorRect.bottom = camera->getCameraView().bottom + camera->getCameraView().height - m_ActorRect.height;
	}
}

void Player::HorizontalColliding(const Object* object)
{
	Point2f p1;
	Point2f p2;
	Point2f middlePoint;

	float offset{ 1 };
	utils::HitInfo hitInfo;
	
	middlePoint = Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 };
	p1 = Point2f{ m_ActorRect.left - offset, m_ActorRect.bottom + m_ActorRect.height / 2 };
	p2 = Point2f{ m_ActorRect.left + m_ActorRect.width + offset, m_ActorRect.bottom + m_ActorRect.height / 2 };

	if (m_Velocity.x < 0)
	{
		//going left
		if (utils::Raycast(object->getVertices(), p1, middlePoint, hitInfo))
		{
			//Border left
			m_Velocity.x = 0;
			m_ActorRect.left = hitInfo.intersectPoint.x + offset * 2;
		}
	}
	else
	{
		//going straight or right
		if (utils::Raycast(object->getVertices(), p2, middlePoint, hitInfo))
		{
			//Border left
			m_Velocity.x = 0;
			m_ActorRect.left = hitInfo.intersectPoint.x - m_ActorRect.width - offset * 2;
		}
	}
}
void Player::VerticalColliding(const Object* object)
{
	Point2f p1;
	Point2f p2;
	Point2f middlePoint;

	float offset{ 1 };
	utils::HitInfo hitInfo;
	
	middlePoint = Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height / 2 };
	p1 = Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom + m_ActorRect.height + offset };
	p2 = Point2f{ m_ActorRect.left + m_ActorRect.width / 2, m_ActorRect.bottom - offset };

	if (m_Velocity.y < 0)
	{
		//going left
		if (utils::Raycast(object->getVertices(), middlePoint, p2, hitInfo))
		{
			//border bottom
			m_Velocity.y = 0;
			m_ActorRect.bottom = hitInfo.intersectPoint.y + offset * 2;
		}
	}
	else
	{
		//going straight or right
		if (utils::Raycast(object->getVertices(), middlePoint, p1, hitInfo))
		{
			//top border
			m_Velocity.y = 0;
			m_ActorRect.bottom = hitInfo.intersectPoint.y - m_ActorRect.height - offset * 2;
		}
	}
}

void Player::KillActor()
{
	if (m_IsHit == false)
	{
		//std::cout << "AW" << std::endl;
		if (m_Health > 0)
		{
			--m_Health;
			m_Timer->setTimeGoal( 1.5f );
			m_IsHit = true;
		}
		else
		{
			//The actor is dead
			m_ResourceManager->Sounds()->PlaySound("Dying");
			m_ActionState = ActionState::dying;
		}
	}	
}
void Player::CollidingWithEnemy(const Enemy* enemy)
{
	if (utils::IsOverlapping(m_ActorRect, enemy->getActorRect()))
	{
		//Enemy hit player
		KillActor();
	}
}

void Player::IncreaseHealth(int amount)
{
	m_Health += amount;
	if (m_Health > m_MaxHealth) m_Health = m_MaxHealth;
}
void Player::IncreaseBombs(int amount)
{
	m_Bombs += amount;
	if (m_Bombs > 3) m_Bombs = 3;
}

int Player::getHealth() const
{
	return int(m_Health);
}
int Player::getMaxHealth() const
{
	return int(m_MaxHealth);
}
int Player::getBombs() const	
{
	return m_Bombs;
}