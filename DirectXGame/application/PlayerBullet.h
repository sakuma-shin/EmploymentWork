#pragma once
#include "BaseObject.h"
#include "KamataEngine.h"
#include "Model2.h"
#include "PlayerBulletParticle.h"

class PlayerBullet : public BaseObject {
public:
	// 初期化
	void Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 更新
	void Update() override;

	// 描画
	void Draw(KamataEngine::Camera& camera) override;

	bool IsDead() const { return isDead_; }

	float GetRadius() { return radius_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

private:
	KamataEngine::Vector3 velocity_ = {};
	float deathTimer_ = 120.0f;
	bool isDead_ = false;
	// 半径
	float radius_ = 0.0f;
};