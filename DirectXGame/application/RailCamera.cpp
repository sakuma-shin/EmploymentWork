#include "RailCamera.h"
#include"Player.h"

using namespace KamataEngine;
using namespace MathUtility;

void RailCamera::Initialize(KamataEngine::Vector3 pos, KamataEngine::Vector3 rotate) { 
	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = rotate;

	startPos_ = worldTransform_.translation_;

	endPos_ = worldTransform_.translation_;
	endPos_.z = 30.0f;

	//カメラの初期化
	camera_.Initialize();
}

void RailCamera::Update() {

	Vector3 velocity = {0.0f, 0.0f, 0.2f};
	worldTransform_.translation_ += velocity;

	Vector3 radian = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ += radian;

	worldTransform_.AffineMatrix();

	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
}

KamataEngine::WorldTransform& RailCamera::GetWorldTransform() { return worldTransform_; }

void RailCamera::StartDirection() { 
	//
	if (timer_ <= kMaxTimer) {
		timer_++;
		if (timer_ <= kMoveTimer) {
			worldTransform_.translation_.z = startPos_.z + (endPos_.z - startPos_.z) * EaseInSine(timer_ / kMaxTimer);
		}
	}
	worldTransform_.AffineMatrix();
	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
}