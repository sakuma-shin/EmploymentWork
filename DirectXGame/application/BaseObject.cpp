#include "BaseObject.h"

using namespace KamataEngine;
using namespace MathUtility;

void BaseObject::BaseInitialize(KamataEngine::Model2* model, uint32_t textureHandle) {
	// モデルは子クラス側でチェックしても良いがここでも受け取って保持する
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	color_.Initialize();
}

void BaseObject::Draw(KamataEngine::Camera& camera) {
	if (!model_) {
		return;
	}
	// textureHandle_ が設定されていればテクスチャ差し替えの Draw を使う
	if (textureHandle_ != 0u) {
		model_->Draw(worldTransform_, camera, textureHandle_, &color_);
	} else {
		model_->Draw(worldTransform_, camera, &color_);
	}
}

KamataEngine::Vector3 BaseObject::GetWorldPosition() {
	KamataEngine::Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}