#include "Player.h"
#include <cassert>

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	input_ = Input::GetInstance();
}

void Player::Update() {

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	const float kSpeed = 0.5f;
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

	const float kMoveLimitX = 12.0f;
	const float kMoveLimitY = 8.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	if (input_->TriggerKey(DIK_SPACE)) {
		PlayerBullet* newBullet = new PlayerBullet();
		float bulletSpeed = 0.6f;
		Vector3 bulletVelocity = {0.0f, 0.0f, bulletSpeed};
		newBullet->Initialize(model_, GetWorldPosition(), bulletVelocity);
		bullets_.push_back(newBullet);
	}

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(KamataEngine::Camera& camera) {
	model_->Draw(worldTransform_, camera); 

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::SetParent(const KamataEngine::WorldTransform* parent) {
//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
