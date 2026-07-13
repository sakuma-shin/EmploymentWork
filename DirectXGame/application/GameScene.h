#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include "IScene.h"
#include<vector>
#include <memory>
#include"Player.h"
#include"RailCamera.h"
#include"skyDome.h"
#include"Enemy.h"
#include"EnemyBullet.h"
#include"Wall.h"
#include"ExMathUtility.h"
#include"Door.h"

/// <summary>
/// ゲームプレイシーンを実装するクラス
/// </summary>
class GameScene:public IScene{
public:
	~GameScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	//<summary>
	// 敵弾を追加する
	//</summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	// 弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return enemyBullets_; }

	//<summary>
	// 敵発生データの読み込み
	//</summary>
	void LoadEnemyPopData();

	//<summary>
	// 敵発生コマンドの更新
	//</summary>
	void UpdateEnemyPopCommands();

	void SpawnEnemy(KamataEngine::Vector3 spawnPos);

	void SpawnWall(KamataEngine::Vector3 spawnPos, KamataEngine::Vector3 spawnScale);

	//<summary>
	// 衝突判定と応答
	//</summary>
	void CheckAllCollisions();

	/// <summary>
	/// スタート演出
	/// </summary>
	void StartDirection();

	/// <summary>
	/// プレイ中の更新処置
	/// </summary>
	void PlayUpdate();


private:
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Input* input_ = nullptr;

	///*player関連*///
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<KamataEngine::Model2> playerModel_ = nullptr;
	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	const int domeNum =50;
	std::vector<std::unique_ptr<SkyDome>> skyDomes_;

	std::unique_ptr<Door> door_ = nullptr;
	std::unique_ptr<KamataEngine::Model2> doorModel_ = nullptr;


	///*enemy関連*///
	std::list<std::unique_ptr<Enemy>> enemies_;
	uint32_t enemyTextureHandle_ = 0u;
	std::unique_ptr<KamataEngine::Model2> enemyModel0_ = nullptr;
	std::unique_ptr<KamataEngine::Model2> enemyModel1_ = nullptr;
	std::unique_ptr<KamataEngine::Model2> enemyModel2_ = nullptr;

	std::unique_ptr<KamataEngine::Model2> skyDomeModel_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	int32_t spawnTimer_ = 0;
	bool isSpawn_ = false;

	// 壁
	std::list<std::unique_ptr<Wall>> walls_;
	std::unique_ptr<KamataEngine::Model2> wallModel_ = nullptr;
	uint32_t wallTextureHandle_ = 0u;

	// デバッグカメラ
	std::unique_ptr<KamataEngine::DebugCamera> debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;

	enum class Phase {
		START,
		PLAY
	};

	Phase phase_ = Phase::START;

	uint32_t startTextureHandle_ = 0u;
	std::unique_ptr<KamataEngine::Sprite> startSprite_ = nullptr;

	uint32_t clearTextureHandle_ = 0u;
	int clearTimer_ = 0;
	std::unique_ptr<KamataEngine::Sprite> clearSprite_ = nullptr;

	std::unique_ptr<KamataEngine::Model2> playerBulletModel_ = nullptr;

	std::unique_ptr<KamataEngine::Model2> EnemyBulletModel_ = nullptr;

	bool isClear = false;

	uint32_t lifeTextureHandle_ = 0u;
	std::vector<std::unique_ptr<Sprite>> lifeSprites_;
};
