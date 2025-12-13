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
