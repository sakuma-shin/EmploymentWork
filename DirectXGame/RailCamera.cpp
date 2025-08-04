#include "RailCamera.h"
using namespace KamataEngine;
using namespace MathUtility;

void RailCamera::Initialize(KamataEngine::Vector3 pos, KamataEngine::Vector3 rotate) { 
	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rotate;

	//カメラの初期化
	camera_.Initialize();
}

void RailCamera::Update() {
	Vector3 velocity = {0.0f, 0.0f, 0.05f};
	worldTransform_.translation_ += velocity;

	Vector3 radian = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ += radian;

	worldTransform_.AffineMatrix();

	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
}

KamataEngine::WorldTransform& RailCamera::GetWorldTransform() { return worldTransform_; }
