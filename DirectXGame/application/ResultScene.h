#pragma once
#include "IScene.h"
#include "KamataEngine.h"
#include "Model2.h"
#include <memory>

/// <summary>
/// 結果画面を管理するクラス
/// </summary>
class ResultScene:public IScene {
public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	~ResultScene();

private:
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Input* input_ = nullptr;

	std::unique_ptr<KamataEngine::Sprite> sprite_ = nullptr;
	uint32_t textureHandle_ = 0u;
};
