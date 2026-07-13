#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include <list>
#include <random>
#include <memory>

/// <summary>
// Particleクラスは単一のパーティクルを表す
/// </summary>
class Particle {
public:
	// 初期化
	void Initialize(const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, float lifeTime);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& camera);

	// 死亡フラグ取得
	bool IsDead() const { return life_ <= 0.0f; }

private:
	KamataEngine::WorldTransform worldTransform_;
	std::unique_ptr<KamataEngine::Model2> model_ = nullptr; // パーティクル専用のモデル（丸など）
	KamataEngine::Vector3 velocity_ = {};

	KamataEngine::ObjectColor color_;

	float life_ = 0.0f;     // 現在の寿命
	float lifeTime_ = 0.0f; // 最大寿命
};