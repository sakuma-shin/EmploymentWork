#include "EnemyBullet.h"

    using namespace KamataEngine;
using namespace MathUtility;

void EnemyBullet::Initialize(Model2* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	BaseInitialize(model, 0u);

	velocity_ = velocity;
	worldTransform_.translation_ = position;
	radius_ = 1.0f;
	deathTimer_ = kLifeTime;
	color_.Initialize();
	color_.SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const Camera& camera) {
	if (model_)
		model_->Draw(worldTransform_, camera, &color_);
}

KamataEngine::Vector3 EnemyBullet::GetWorldPosition() { return BaseObject::GetWorldPosition(); }

void EnemyBullet::OnCollision() { isDead_ = true; }