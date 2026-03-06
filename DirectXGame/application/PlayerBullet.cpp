#include "PlayerBullet.h"

using namespace KamataEngine;
using namespace MathUtility;

void PlayerBullet::Initialize(Model2* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	BaseInitialize(model, 0u);

	velocity_ = velocity;
	worldTransform_.translation_ = position;
	radius_ = 1.0f;
	deathTimer_ = 120.0f;

	color_.Initialize();
	color_.SetColor({0.29f, 0.0f, 0.42f, 1.0f});
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera& camera) {
	if (model_)
		model_->Draw(worldTransform_, camera, &color_);
}

KamataEngine::Vector3 PlayerBullet::GetWorldPosition() { return BaseObject::GetWorldPosition(); }

void PlayerBullet::OnCollision() { isDead_ = true; }