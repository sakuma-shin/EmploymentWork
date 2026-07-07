#pragma once
#include "KamataEngine.h"
#include "Model2.h"

/// <summary>
/// ステージクリア後に演出として開く扉を実装するクラス
/// </summary>
class Door {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model2* model,KamataEngine::Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera);

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model2* model_ = nullptr;
};
