#pragma once
#include "KamataEngine.h"
#include "Model2.h"
class SkyDome {
public:
	void Initialize(KamataEngine::Model2* model,float positionZ);

	void Update();

	void Draw(KamataEngine::Camera& camera);

	KamataEngine::Vector3 GetWorldPosition();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_;
};
