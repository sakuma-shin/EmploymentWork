#include "Player.h"
#include <cassert>
#include "Easing.h"

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	
	size_ = {2.0f, 8.0f, 2.0f};

	input_ = Input::GetInstance();

	life = kMaxLife;

	isDead_ = false;

	worldTransform_.UpdateMatrix();

	state_ = State::kPlay;

	deathTimer_ = 0.0f;

	color_.Initialize();
}

void Player::Update(KamataEngine::Model2* model) {

	switch (state_) {
	case State::kPlay:
		PlayUpdate(model);

		break;

	case State::kDeathRotate:
		DeathRotate();
		/*color_.SetColor({0.0f, 0.0f, 0.0f, 1.0f});*/
		break;
	case State::kDeathDrop:
		DeathDrop();
		break;
	case State::kDeathDisappear:
		DeathDisappear();
		break;
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(KamataEngine::Camera& camera) {

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}

	model_->Draw(worldTransform_, camera,&color_);
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::SetParent(const KamataEngine::WorldTransform* parent) {
	// 親子関係を結ぶ
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

void Player::OnCollision() { life -= 1; }

void Player::PlayUpdate(KamataEngine::Model2* model) {
	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	const float kSpeed = 0.5f;
	// プレイヤーの移動量を格納する変数
	Vector3 move = {0.0f, 0.0f, 0.0f};

	if (input_->PushKey(DIK_A)) {
		move.x -= kSpeed; // 移動量を累積
	}

	if (input_->PushKey(DIK_D)) {
		move.x += kSpeed; // 移動量を累積
	}

	if (input_->PushKey(DIK_W)) {
		move.y += kSpeed; // 移動量を累積
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kSpeed; // 移動量を累積
	}

	// ワールド座標の更新
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;

	// プレイヤーの傾きを設定するための定数
	const float kMaxTilt = 0.3f;   // 最大傾き角度（ラジアン）
	const float kTiltSpeed = 0.2f; // 傾きが目標角度に近づく速さ（大きいほど速い）

	// 目標とする回転角度を計算
	// X軸の回転（上下の傾き）: 上に移動 (move.y > 0) で -kMaxTilt (奥に傾く), 下に移動 (move.y < 0) で +kMaxTilt (手前に傾く)
	float targetRotX = -move.y / kSpeed * kMaxTilt;
	// Z軸の回転（左右の傾き）: 右に移動 (move.x > 0) で -kMaxTilt (右に傾く), 左に移動 (move.x < 0) で +kMaxTilt (左に傾く)
	float targetRotZ = -move.x / kSpeed * kMaxTilt;

	// 現在の回転角度を目標角度にLerp（線形補間）で近づける
	// こうすることで、キーを押した瞬間ではなく滑らかに傾きが変化します
	worldTransform_.rotation_.x += (targetRotX - worldTransform_.rotation_.x) * kTiltSpeed;
	worldTransform_.rotation_.z += (targetRotZ - worldTransform_.rotation_.z) * kTiltSpeed;



	if (input_->TriggerKey(DIK_R)) {
		life -=1;
	}

	const float kMoveLimitX = 12.0f;
	const float kMoveLimitY = 8.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	if (input_->TriggerKey(DIK_SPACE)) {
		// 弾
		PlayerBullet* newBullet = new PlayerBullet();
		float bulletSpeed = 0.6f;
		Vector3 bulletVelocity = {0.0f, 0.0f, bulletSpeed};
		newBullet->Initialize(model, GetWorldPosition(), bulletVelocity);
		bullets_.push_back(newBullet);
	}

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	if (life <= 0) {
		state_ = State::kDeathRotate;
	}

	float color = 1.0f;

	color = static_cast<float>(life) / kMaxLife;

	color_.SetColor({color, color, color, 1.0f});
}

void Player::DeathRotate() { 
	float startRot = worldTransform_.rotation_.x;

	float endRot = 1.0f;

	deathTimer_++;

	if (deathTimer_ <= MaxDeathTimer[0]) {
		deathTimer_++;
		worldTransform_.rotation_.x = startRot + (endRot - startRot) * EaseInSine(deathTimer_ / MaxDeathTimer[0]);
	} else {
		deathTimer_ = 0.0f;
		state_ = State::kDeathDrop;
	}

}

void Player::DeathDrop() { 
	float startPos = worldTransform_.translation_.y;

	float endPos = startPos;

	if (deathTimer_ <= MaxDeathTimer[1]) {
		deathTimer_++;
		worldTransform_.translation_.y = startPos + (endPos - startPos) * EaseInSine(deathTimer_ / MaxDeathTimer[1]);
	} else {
		deathTimer_ = 0.0f;
		state_ = State::kDeathDisappear;
	}

}

void Player::DeathDisappear() {
	Vector3 startScale = worldTransform_.scale_;

	Vector3 endScale = {0.0f,0.0f,0.0f};

	if (deathTimer_ <= MaxDeathTimer[1]) {
		deathTimer_++;
		worldTransform_.scale_.x = startScale.x + (endScale.x - startScale.x) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
		worldTransform_.scale_.y = startScale.y + (endScale.y - startScale.y) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
		worldTransform_.scale_.z = startScale.z + (endScale.z - startScale.z) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
	} else {
		isDead_ = true;
	}
}
