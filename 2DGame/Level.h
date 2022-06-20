#pragma once
class Camera;
class Texture;
class SVGParser;
class Enemy;
class Player;
class Actor;
class ArcadeMachine;
class BossClass;
class HUD;
class EnemySpawner;

#include <vector>
#include "Timer.h"
#include "ResourceManager.h"

class Level final
{
public:
	enum class Direction{ horizontal, vertical };

	Level(Rectf window, float scale, ResourceManager* resourceManager);
	~Level();

	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	void HandleCollision( Rectf& actorShape, Vector2f& velocity, Direction direction, int leftOrRight) const;
	bool IsTouchingBorder(const Rectf& actorShape, Direction direction, int leftOrRight) const;
	
	Rectf getMapRect() const;

	void ImportActor(Actor* actor);
	void LoadMap( std::map<std::string, std::string> mapPaths);

	void Reset();

	bool HasKilledBoss() const;
	bool isGameOver() const;

private:
	//Map
	Texture* m_Texture;
	SVGParser* m_LevelSVG;
	std::vector<std::vector<Point2f>> m_Vertices;
	float m_Scale;

	//Window
	Rectf m_Window;

	//Resources
	ResourceManager* m_ResourceManager;

	//Players
	Player* m_pPlayerOne;
	Player* m_pPlayerTwo;
	int m_PlayerCount;
	const int m_MaxPlayers;

	//Actors
	std::vector<Actor*> m_Actors;

	//Enemy
	int m_EnemyCount;
	std::vector<EnemySpawner*> m_EnemySpawners;

	//Boss
	BossClass* m_LevelBoss;

	//HUD
	ArcadeMachine* m_Arcade;
	HUD* m_HUD;

	//Score
	int m_Score;

	int m_EnemyScore;
	int m_ObjectScore;
	int m_BossScore;

	//Camera
	Camera* m_PlayerCamera;

	//Functions
	void SVGMapLoading(std::string SVGfilePath);
	void DrawMapTexture(float scale) const;

	float getLength( Point2f p1, Point2f p2 );

	void UpdateHUD();

	void InitializeHUD();
	void AddActors();
	void InitializeCamera();

	void CheckForActorDeath();
};

