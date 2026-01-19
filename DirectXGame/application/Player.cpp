#include "Player.h"
#include "Easing.h"
#include <cassert>


using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ *= 0.5f;

	size_ = {1.0f, 4.0f, 1.0f};

	input_ = Input::GetInstance();

	life = kMaxLife;

	isDead_ = false;

	worldTransform_.UpdateMatrix();

	state_ = State::kPlay;

	deathTimer_ = 0.0f;

	color_.Initialize();

	canShot_ = true;

	invTime_ = kMaxInvtime;
}

void Player::Update(KamataEngine::Model2* model) {

	switch (state_) {
	case State::kPlay:
		PlayUpdate(model);

		break;

	case State::kDeathRotate:
		DeathRotate();
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

	for (PlayerBulletParticle* particle : inkParticles_) {
		particle->Draw(camera);
	}

	model_->Draw(worldTransform_, camera, &color_);
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

void Player::OnCollision() {
	life -= 1;
	isDamaged_ = true;
}

void Player::PlayUpdate(KamataEngine::Model2* model) {
	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	inkParticles_.remove_if([](PlayerBulletParticle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
	});

	const float kSpeed = 0.5f;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	if (input_->PushKey(DIK_A)) {
		move.x -= kSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		move.x += kSpeed;
	}

	if (input_->PushKey(DIK_W)) {
		move.y += kSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		move.y -= kSpeed;
	}

	//// ワールド座標の更新
	// worldTransform_.translation_.x += move.x;
	// worldTransform_.translation_.y += move.y;

	const float kMaxTilt = 0.3f;
	const float kTiltSpeed = 0.2f;

	float targetRotX = -move.y / kSpeed * kMaxTilt;

	float targetRotZ = -move.x / kSpeed * kMaxTilt;

	worldTransform_.rotation_.x += (targetRotX - worldTransform_.rotation_.x) * kTiltSpeed;
	worldTransform_.rotation_.z += (targetRotZ - worldTransform_.rotation_.z) * kTiltSpeed;

	// if (input_->TriggerKey(DIK_R)) {
	//	life -= 1;
	// }

	if (canShot_) {
		if (input_->TriggerKey(DIK_SPACE)) {
			// 弾
			PlayerBullet* newBullet = new PlayerBullet();
			float bulletSpeed = 2.0f;
			Vector3 bulletVelocity = {0.0f, 0.0f, bulletSpeed};
			newBullet->Initialize(model, GetWorldPosition(), bulletVelocity);
			bullets_.push_back(newBullet);
			canShot_ = false;
		}
	} else {
		fireTimer_--;
		if (fireTimer_ <= 0) {
			canShot_ = true;
			fireTimer_ = kMaxFireTimer_;
		}
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

	float alpha = 1.0f;
	if (isDamaged_) {
		if (--invTime_ <= 0) {
			isDamaged_ = false;
			invTime_ = kMaxInvtime;
		}

		if (invTime_ % 6 == 0) {
			alpha = 0.0f;
		}
	}

	color_.SetColor({color, color, color, alpha});

	// particleの更新
	for (PlayerBulletParticle* particle : inkParticles_) {
		particle->Update();
	}
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

	Vector3 endScale = {0.0f, 0.0f, 0.0f};

	if (deathTimer_ <= MaxDeathTimer[1]) {
		deathTimer_++;
		worldTransform_.scale_.x = startScale.x + (endScale.x - startScale.x) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
		worldTransform_.scale_.y = startScale.y + (endScale.y - startScale.y) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
		worldTransform_.scale_.z = startScale.z + (endScale.z - startScale.z) * EaseInBounce(deathTimer_ / MaxDeathTimer[2]);
	} else {
		isDead_ = true;
	}
}

void Player::GenerateParticle() {
	for (PlayerBullet* bullet : bullets_) {
		PlayerBulletParticle* newParticle = new PlayerBulletParticle();
		newParticle->Initialize(model_, bullet->GetWorldPosition());
		inkParticles_.push_back(newParticle);
	}
}
