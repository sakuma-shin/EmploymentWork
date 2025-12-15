#include "Door.h"

using namespace KamataEngine;
using namespace MathUtility;
void Door::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 pos) {

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();

	model_=model;

}

void Door::Update() {

}

void Door::Draw(KamataEngine::Camera& camera) { model_->Draw(worldTransform_, camera); }