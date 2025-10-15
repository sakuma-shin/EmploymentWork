#include"Particle.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;
using namespace MathUtility;

void Particle::Initialize(const Vector3& position, const Vector3& velocity, float lifeTime) {
	model_ = Model2::CreateSphere(4, 4);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ *=0.1f ;
	velocity_ = velocity;
	lifeTime_ = lifeTime;
	life_ = lifeTime; // 寿命を初期化
}

void Particle::Update() {
	// 寿命を減らす
	life_ -= 1.0f; // 1フレームごとに1減らす

	// 座標を速度で移動させる
	worldTransform_.translation_ += velocity_;

	// 速度を減衰させる
	velocity_ *= 0.98f;


	float scale = life_ / lifeTime_;
	worldTransform_.scale_ *= scale;

	worldTransform_.UpdateMatrix();
}

void Particle::Draw(Camera& camera) {
	if (model_) {
		model_->Draw(worldTransform_, camera);
	}
}