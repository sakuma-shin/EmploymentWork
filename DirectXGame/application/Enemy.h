#pragma once
#include <cassert>
#include <list>
#include "KamataEngine.h"
#include"Model2.h"
#include "IScene.h"
#include<vector>

class Player;
class GameScene;

/// <summary>
/// 敵を実装するクラス
/// </summary>
class Enemy {
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 敵の初期化
	/// </summary>
	/// <param name="models 使用するモデル"></param>
	/// <param name="textureHandle モデルに張り付けるテクスチャ"></param>
	/// <param name="position" 発生させる位置></param>
	/// <param name="bulletModel 弾のモデル"></param>
	void Initialize(std::vector<KamataEngine::Model2*>& models, uint32_t textureHandle, const KamataEngine::Vector3& position, KamataEngine::Model2* bulletModel);

	/// <summary>
	/// 敵の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 敵の描画	
	/// </summary>
	/// <param name="camera"></param>
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

	//アニメーションの切り替え間隔（フレーム数）
	static const int kAnimationInterval = 10;

	KamataEngine::Vector3 GetSize() { return size_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

	Phase GetPhase() const { return phase_; }

	void Dead();

private:
	KamataEngine::WorldTransform worldTransform_;

	// modelのリスト
	std::vector<KamataEngine::Model2*> models_;

	//現在のアニメーションフレームのインデックス
	int32_t currentModelIndex_ = 0;
	//アニメーション用タイマー
	int32_t animationTimer_ = 0;

	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 position_ = {};

	KamataEngine::Vector3 velocity_ = {};

	Phase phase_ = Phase::Approach;

	static const int kLeaveDuration = 60;
	int32_t leaveTimer_ = 0;
	const int32_t kMaxLeaveTimer_ = 60;

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

	KamataEngine::ObjectColor color_;

};