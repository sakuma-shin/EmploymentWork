#include "Wall.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

void Wall::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position, KamataEngine::Vector3 scale, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	// スケールから半径を簡易的に算出
	radius_ = (scale.x + scale.y);

	isDead_ = false;

	worldTransform_.UpdateMatrix();
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
