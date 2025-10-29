#pragma once

#include "GameScene.h"
#include "IScene.h"
#include "TitleScene.h"
#include"ResultScene.h"
#include <memory>
#include"KamataEngine.h"
using namespace KamataEngine;

class GameManager {
private:
	// シーンを保持するメンバ関数
	std::unique_ptr<IScene> sceneArr_[3];

	int currentSceneNo_;
	int prevSceneNo_;

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = nullptr;

	Input* input_ = nullptr;


	// DirectXCommonクラスが管理している、ウインドウの幅と高さの取得
	int32_t w = 0u;
	int32_t h = 0u;

public:
	GameManager();
	~GameManager();

	int Run();
};
