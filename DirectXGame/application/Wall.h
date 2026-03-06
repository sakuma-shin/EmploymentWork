#pragma once
#include "BaseObject.h"
#include "KamataEngine.h"
#include "Model2.h"
#include <cstdint>

class Wall : public BaseObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& scale, uint32_t textureHandle);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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
	// worldTransform_ / model_ / textureHandle_ / color_ は BaseObject に移動
	KamataEngine::Vector3 size_ = {};
	bool isDead_ = false;
};