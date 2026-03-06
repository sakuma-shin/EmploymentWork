#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "BaseObject.h"

class Door : public BaseObject {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(KamataEngine::Camera& camera) override;
private:

};
