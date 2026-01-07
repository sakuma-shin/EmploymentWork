#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include <cstdint>

class Wall {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position, KamataEngine::Vector3 scale, uint32_t textureHandle);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	void OnCollision() { isDead_ = true; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// 半径のGetter
	KamataEngine::Vector3 GetSize() { return size_; }

	// デスフラグのGetter
	bool IsDead() const { return isDead_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 size_ = {};
	
	// デスフラグ
	bool isDead_ = false;
};