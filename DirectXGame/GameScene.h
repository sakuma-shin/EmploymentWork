#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include "IScene.h"
#include<vector>
#include"Player.h"
#include"RailCamera.h"
#include"skyDome.h"

class GameScene:public IScene{
public:
	~GameScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;

private:
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Input* input_ = nullptr;

	///*player関連*///
	Player* player_ = nullptr;
	KamataEngine::Model2* playerModel_ = nullptr;
	RailCamera* railCamera_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	KamataEngine::Model2* skyDomeModel_ = nullptr;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;
};
