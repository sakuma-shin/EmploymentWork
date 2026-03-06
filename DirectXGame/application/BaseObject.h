#pragma once
#include "KamataEngine.h"
#include "Model2.h"

/// <summary>
/// ゲーム内オブジェクトの共通親クラス
/// - WorldTransform, Model2*, textureHandle, ObjectColor を共通化
/// - 初期化補助(BaseInitialize) と基本的な描画を提供
/// </summary>
class BaseObject {
public:
	virtual ~BaseObject() = default;

	// 基本初期化（子クラスの Initialize 内で呼ぶ）
	void BaseInitialize(KamataEngine::Model2* model, uint32_t textureHandle = 0u);

	// 更新 / 描画（子クラスでオーバーライド可能）
	virtual void Update() {}
	virtual void Draw(KamataEngine::Camera& camera);

	// ワールド座標の取得（子クラスで共通利用）
	KamataEngine::Vector3 GetWorldPosition();

	// 親トランスフォームをセット（必要な場合）
	void SetParent(const KamataEngine::WorldTransform* parent) { worldTransform_.parent_ = parent; }

protected:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::ObjectColor color_;
};