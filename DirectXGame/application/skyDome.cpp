#include "skyDome.h"
#include<cassert>
using namespace KamataEngine;
using namespace MathUtility;

void SkyDome::Initialize(KamataEngine::Model2* model, float positionZ) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ *= 15.0f;
	worldTransform_.translation_.z = positionZ;
	worldTransform_.UpdateMatrix();
}

void SkyDome::Draw(Camera& camera) { model_->Draw(worldTransform_, camera); }

Vector3 SkyDome::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
