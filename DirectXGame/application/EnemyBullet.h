#pragma once
#include "BaseObject.h"
#include "KamataEngine.h"
#include "Model2.h"
#include <cassert>

    using namespace KamataEngine;
using namespace MathUtility;

class EnemyBullet : public BaseObject {
public:
	void Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void Update() override;

	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_; }

	// 衝突を検知したら呼び出される関数
	void OnCollision();

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	float GetRadius() { return radius_; }

private:
	KamataEngine::Vector3 velocity_;
	// 弾の寿命
	static const uint32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	float radius_ = 0.0f;
};