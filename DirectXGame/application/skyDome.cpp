#include "skyDome.h"
#include<cassert>
using namespace KamataEngine;

void SkyDome::Initialize(KamataEngine::Model2* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.1f, 0.1f, 1.0f};
	worldTransform_.UpdateMatrix();
}

void SkyDome::Draw(Camera& camera) { model_->Draw(worldTransform_, camera); }
