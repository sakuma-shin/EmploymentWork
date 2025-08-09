#include "skyDome.h"
#include<cassert>
using namespace KamataEngine;

void SkyDome::Initialize(KamataEngine::Model2* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.4f, 0.4f, 20.0f};
}

void SkyDome::Draw(Camera& camera) { model_->Draw(worldTransform_, camera); }
