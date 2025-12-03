#pragma once
#include <cassert>
#include <list>
#include "KamataEngine.h"
#include"Model2.h"
#include "IScene.h"
#include<vector>

class Player;
class GameScene;

class Enemy {
public:
	Enemy();
	~Enemy();

	void Initialize(KamataEngine::Model2* model, uint32_t textureHandle, const KamataEngine::Vector3& position,KamataEngine::Model2* bulletModel);

	void Update();

	void Draw(KamataEngine::Camera& camera);

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	///< summary>
	/// 敵が近づく
	///</summary>
	void Approach();

	///< summary>
	/// 敵が離脱する
	///</summary>
	void Leave();

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

	void Fire();

	// 接近フェーズの初期化
	void PhaseInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	KamataEngine::Vector3 GetWorldPosition();

	// 発射感覚
	static const int kFireInterval = 180;

	KamataEngine::Vector3 GetSize() { return size_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model2* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 position_ = {};

	KamataEngine::Vector3 velocity_ = {};

	Phase phase_ = Phase::Approach;

	static const int kLeaveDuration = 60;
	int32_t leaveTimer_ = 0;
	KamataEngine::Vector3 initialTranslation_ = {};
	float flyUpHeight_ = 1.5f; 

	int32_t fireTimer_ = 0;

	// 自キャラ
	Player* player_ = nullptr;

	//サイズ
	KamataEngine::Vector3 size_ = {};

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;

	KamataEngine::Model2* bulletModel_;
};