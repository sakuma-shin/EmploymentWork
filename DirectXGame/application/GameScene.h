#pragma once
#include "Door.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "ExMathUtility.h"
#include "IScene.h"
#include "KamataEngine.h"
#include "Model2.h"
#include "Player.h"
#include "RailCamera.h"
#include "Wall.h"
#include "skyDome.h"
#include <list>
#include <memory>
#include <sstream>
#include <vector>

class GameScene : public IScene {
public:
	~GameScene() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;

	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();
	void SpawnEnemy(KamataEngine::Vector3 spawnPos);
	void SpawnWall(KamataEngine::Vector3 spawnPos, KamataEngine::Vector3 spawnScale);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera camera_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<KamataEngine::Model2> playerModel_;
	std::unique_ptr<RailCamera> railCamera_;

	const int domeNum = 50;
	std::vector<std::unique_ptr<SkyDome>> skyDomes_;
	std::unique_ptr<Door> door_;
	std::unique_ptr<KamataEngine::Model2> doorModel_;

	std::list<std::unique_ptr<Enemy>> enemies_;
	uint32_t enemyTextureHandle_ = 0u;
	std::unique_ptr<KamataEngine::Model2> enemyModel0_;
	std::unique_ptr<KamataEngine::Model2> enemyModel1_;
	std::unique_ptr<KamataEngine::Model2> enemyModel2_;
	std::unique_ptr<KamataEngine::Model2> skyDomeModel_;

	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	std::stringstream enemyPopCommands;

	int32_t spawnTimer_ = 0;
	bool isSpawn_ = false;

	std::list<std::unique_ptr<Wall>> walls_;
	std::unique_ptr<KamataEngine::Model2> wallModel_;
	uint32_t wallTextureHandle_ = 0u;

	std::unique_ptr<KamataEngine::DebugCamera> debugCamera_;
};