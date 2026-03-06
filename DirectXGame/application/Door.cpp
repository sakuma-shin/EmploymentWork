#include "Door.h"
#include <cassert>

using namespace KamataEngine;
using namespace MathUtility;

void Door::Initialize(Model2* model, Vector3 pos) {
	assert(model);
	BaseInitialize(model, 0u);
	worldTransform_.translation_ = pos;
	worldTransform_.UpdateMatrix();
}

void Door::Update() {
	worldTransform_.UpdateMatrix();
}

void Door::Draw(Camera& camera) { BaseObject::Draw(camera); }