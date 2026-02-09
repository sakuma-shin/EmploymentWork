#include "Wall.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

void Wall::Initialize(KamataEngine::Model2* model, uint32_t textureHandle) {
	BaseInitialize(model, textureHandle);

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	size_ = {15.0f, 20.0f, 1.0f};
}

void Wall::Draw(KamataEngine::Camera& camera) { model_->Draw(worldTransform_, camera); }

void Wall::Update() {

	worldTransform_.UpdateMatrix();
}

Vector3 Wall::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
