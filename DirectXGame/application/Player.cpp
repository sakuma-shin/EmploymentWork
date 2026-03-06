#include "Player.h"
#include "Easing.h"
#include <cassert>

    using namespace KamataEngine;
using namespace MathUtility;

void Player::Initialize(Model2* model, Vector3 position, Vector3 rotate) {
	assert(model);
	BaseInitialize(model, 0u);

	worldTransform_.translation_ = position;
	worldTransform_.scale_ *= 0.5f;
	worldTransform_.rotation_ = rotate;

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

void Player::Update(Model2* model) {
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

void Player::Draw(Camera& camera) {
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(camera);
	}

	for (PlayerBulletParticle* particle : inkParticles_) {
		particle->Draw(camera);
	}

	if (model_)
		model_->Draw(worldTransform_, camera, &color_);
}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	for (PlayerBulletParticle* particle : inkParticles_) {
		delete particle;
	}
}

void Player::SetParent(const KamataEngine::WorldTransform* parent) { worldTransform_.parent_ = parent; }

KamataEngine::Vector3 Player::GetWorldPosition() { return BaseObject::GetWorldPosition(); }

void Player::OnCollision() {
	life -= 1;
	isDamaged_ = true;
}