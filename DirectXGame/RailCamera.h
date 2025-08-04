#pragma once
#include"KamataEngine.h"
class RailCamera {
public:
	// 初期化
	void Initialize(KamataEngine::Vector3 pos,KamataEngine::Vector3 rotate);

	// 更新
	void Update();

	//ワールドトランスフォームのゲッター
	KamataEngine::WorldTransform& GetWorldTransform();

	KamataEngine::Camera& GetCamera() { return camera_; }

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

};
