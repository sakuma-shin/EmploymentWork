#include "skyDome.h"
#include <cassert>

using namespace KamataEngine;
using namespace MathUtility;

void SkyDome::Initialize(Model2* model, float positionZ) {
	assert(model);
	BaseInitialize(model, 0u);
	worldTransform_.scale_ *= 15.0f;
	worldTransform_.translation_.z = positionZ;
	worldTransform_.UpdateMatrix();
}

void SkyDome::Update() { worldTransform_.UpdateMatrix(); }

void SkyDome::Draw(Camera& camera) {
	if (model_)
		model_->Draw(worldTransform_, camera);
}

KamataEngine::Vector3 SkyDome::GetWorldPosition() { return BaseObject::GetWorldPosition(); }