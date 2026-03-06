#include "Enemy.h"
#include "Easing.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "Player.h"

    using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::Initialize(std::vector<Model2*>& models, uint32_t textureHandle, const Vector3& position, Model2* bulletModel) {
	models_ = models;
	bulletModel_ = bulletModel;
	textureHandle_ = textureHandle;

	// BaseObject::worldTransform_ を使う
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.x = 3.0f;
	worldTransform_.rotation_.y = 1.6f;
	worldTransform_.scale_ *= 0.7f;

	PhaseInitialize();

	size_ = {4.0f, 4.0f, 1.0f};

	leaveTimer_ = kMaxLeaveTimer_;

	color_.Initialize();

	isDead_ = false;

	worldTransform_.UpdateMatrix();
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (!models_.empty()) {
		if (phase_ == Phase::Approach) {
			models_[currentModelIndex_]->Draw(worldTransform_, camera, textureHandle_, &color_);
		} else if (phase_ == Phase::Leave) {
			models_[0]->Draw(worldTransform_, camera, textureHandle_, &color_);
		}
	}
}

void Enemy::OnCollision() { phase_ = Phase::Leave; }