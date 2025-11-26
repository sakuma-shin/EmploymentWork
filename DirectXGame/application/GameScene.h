#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include "IScene.h"
#include<vector>
#include"Player.h"
#include"RailCamera.h"
#include"skyDome.h"
#include"Enemy.h"
#include"EnemyBullet.h"

class GameScene:public IScene{
public:
	~GameScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

	//<summary>
	// 敵弾を追加する
	//</summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	//<summary>
	// 敵発生データの読み込み
	//</summary>
	void LoadEnemyPopData();

	//<summary>
	// 敵発生コマンドの更新
	//</summary>
	void UpdateEnemyPopCommands();

	void SpawnEnemy(KamataEngine::Vector3 spawnPos);

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
	Player* player_ = nullptr;
	KamataEngine::Model2* playerModel_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	///*enemy関連*///
	std::list<Enemy*> enemies_;
	uint32_t enemyTextureHandle_ = 0u;
	KamataEngine::Model2* enemyModel_ = nullptr;

	KamataEngine::Model2* skyDomeModel_ = nullptr;

	std::list<EnemyBullet*> enemyBullets_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	int32_t spawnTimer_ = 0;
	bool isSpawn_ = false;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;

	enum class Phase {
		START,
		PLAY
	};

	Phase phase_ = Phase::START;

	uint32_t startTextureHandle_ = 0u;
	KamataEngine::Sprite* startSprite_ = nullptr;

	uint32_t clearTextureHandle_ = 0u;
	int clearTimer_ = 0;
	KamataEngine::Sprite* clearSprite_ = nullptr;

	bool isClear = false;
};
