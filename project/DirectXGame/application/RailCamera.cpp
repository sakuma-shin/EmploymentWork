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
	endPos_.z = 0.0f;

	//カメラの初期化
	camera_.Initialize();

	input_ = Input::GetInstance();
}

void RailCamera::Update() {

	float kSpeed = 0.5f;

	if (input_->PushKey(DIK_A)) {
	    worldTransform_.translation_.x -= kSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += kSpeed;
	}

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += kSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= kSpeed;
	}

	Vector3 velocity = {0.0f, 0.0f, 1.0f};
	worldTransform_.translation_ += velocity;

	Vector3 radian = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ += radian;

	const float kMoveLimitX = 12.0f;
	const float kMoveLimitY = 8.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

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
			worldTransform_.translation_.z = startPos_.z + (endPos_.z - startPos_.z) * EaseOutBounce(timer_ / kMaxTimer);
		}
	}
	worldTransform_.AffineMatrix();
	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
}