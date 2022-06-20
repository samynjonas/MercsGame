#pragma once
#include "Actor.h"
class Player;

class PowerUp final: public Actor
{
public:
	enum class PowerUpType { Power, Food, Health, Bomb, none };

	PowerUp(Rectf actorRect, Rectf window, PowerUpType powerUpType, ResourceManager* resourceManager);
	virtual ~PowerUp();

	PowerUp(const PowerUp& other) = delete;
	PowerUp& operator=(const PowerUp& other) = delete;
	PowerUp(PowerUp&& other) = delete;
	PowerUp& operator=(PowerUp&& other) = delete;

	virtual void Update( float elapsedSec, const Level* level) override;

	void CheckPlayerColliding(Player* player);


private:
	PowerUpType m_PowerUpType;

	void InitializeTexture();
};

