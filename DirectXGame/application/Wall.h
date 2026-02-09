#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include <cstdint>
#include "BaseObject.h"

class Wall: public BaseObject{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="model"></param>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="textureHandle"></param>
	void Initialize(KamataEngine::Model2* model, uint32_t textureHandle) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="camera"></param>
	void Draw(KamataEngine::Camera& camera)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

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