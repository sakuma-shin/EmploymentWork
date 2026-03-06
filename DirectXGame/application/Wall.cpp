#include "Wall.h"
#include <cassert>
#include <cmath>

    using namespace KamataEngine;

void Wall::Initialize(Model2* model, const Vector3& position, const Vector3& scale, uint32_t textureHandle) {
	BaseInitialize(model, textureHandle);

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	size_ = {15.0f, 20.0f, 1.0f};

	worldTransform_.UpdateMatrix();
}

void Wall::Draw(Camera& camera) {
	if (model_) {
		// テクスチャ差し替えの必要がない場合は BaseObject::Draw を使っても良い
		model_->Draw(worldTransform_, camera);
	}
}

void Wall::Update() { worldTransform_.UpdateMatrix(); }

KamataEngine::Vector3 Wall::GetWorldPosition() { return BaseObject::GetWorldPosition(); }