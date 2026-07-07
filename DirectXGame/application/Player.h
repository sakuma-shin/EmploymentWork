#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "PlayerBullet.h"
#include "PlayerBulletParticle.h"
#include <list>

/// <summary>
/// プレイヤーを実装するクラス
/// </summary>
class Player {
public:

	enum class State { kPlay, kDeathRotate, kDeathDrop, kDeathDisappear };

	void Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position,KamataEngine::Vector3 rotate);

	void Update(KamataEngine::Model2* model);

	void Draw(KamataEngine::Camera& camera);

	~Player();

	// 親となるワールドトランスフォームをセット
	void SetParent(const KamataEngine::WorldTransform* parent);

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 半径のGetter
	KamataEngine::Vector3 GetSize() { return size_; }

	bool IsDead() const { return isDead_; }

	void PlayUpdate(KamataEngine::Model2* model);

	void DeathRotate();

	void DeathDrop();

	void DeathDisappear();

	State GetState() const { return state_; }

	bool IsDamaged() const { return isDamaged_; }

	void GenerateParticle();

	void UpdateMatrix() { worldTransform_.UpdateMatrix(); }

	const int kMaxLife = 3;

	int GetLife() const { return life; }

private:
	KamataEngine::Input* input_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model2* model_ = nullptr;
	
	//大きさ
	KamataEngine::Vector3 size_ = {};

	int life = 0;

	bool isDead_ = false;

	std::list<PlayerBullet*> bullets_;

	State state_ = State::kPlay;

	float deathTimer_ = 0.0f;
	
	const float MaxDeathTimer[3] = {
	    // 回転するフレーム
	    30.0f,
	    // 落ちるフレーム
	    30.0f,
	    // 縮まるフレーム
	    60.0f };

	KamataEngine::ObjectColor color_;

	const int32_t kMaxFireTimer_ = 45;
	int32_t fireTimer_ = 0;

	bool canShot_ = false;

	bool isDamaged_ = false;

	int invTime_ = 0;
	const int kMaxInvtime = 45;

	std::list<PlayerBulletParticle*> inkParticles_;
};
