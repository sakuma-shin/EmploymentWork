#include "RailCamera.h"
#include"Player.h"
#include "GameConstants.h"

using namespace KamataEngine;
using namespace MathUtility;
using namespace GameConstants;

void RailCamera::Initialize(KamataEngine::Vector3 startPos, KamataEngine::Vector3 endPos, KamataEngine::Vector3 rotate) { 
	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = startPos;
	worldTransform_.rotation_ = rotate;

	startPos_ = worldTransform_.translation_;

	endPos_ = endPos;

	//カメラの初期化
	camera_.Initialize();

	input_ = Input::GetInstance();
}

void RailCamera::Update() {


	float kSpeed = RAIL_CAMERA_MOVE_SPEED;

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
	const float kMoveLimitUpY = 14.0f;
	const float kMoveLimitDownY = -5.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitUpY);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, kMoveLimitDownY);

	// シェイクタイマーの更新
	if (shakeTimer_ > 0.0f) {
		shakeTimer_ -= 1.0f;
	}
	worldTransform_.AffineMatrix();

	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	Matrix4x4 renderMat = worldTransform_.matWorld_;

	if (shakeTimer_ > 0.0f) {
		// 強さを減衰させる
		float ratio = shakeTimer_ / kMaxShakeTimer;
		float currentIntensity = shakeIntensity_ * ratio;

		// ランダムなオフセットを適用（XとY方向）
		renderMat.m[3][0] += ((float)rand() / RAND_MAX * 2.0f - 1.0f) * currentIntensity;
		renderMat.m[3][1] += ((float)rand() / RAND_MAX * 2.0f - 1.0f) * currentIntensity;
	}

	camera_.matView = Inverse(renderMat);
}

KamataEngine::WorldTransform& RailCamera::GetWorldTransform() { return worldTransform_; }

void RailCamera::StartDirection() { 
	//
	if (timer_ <= kMaxTimer) {
		timer_++;
		if (timer_ <= kMoveTimer) {
			worldTransform_.translation_.z = startPos_.z + (endPos_.z - startPos_.z) * EaseInOutCubic(timer_ / kMoveTimer);
		}
	}
	worldTransform_.AffineMatrix();
	// カメラのオブジェクトのワールド行列からビュー行列を計算する
	camera_.matView = Inverse(worldTransform_.matWorld_);
}
