#pragma once
#include "KamataEngine.h"
#include "Model2.h"
class SkyDome {
public:
	void Initialize(KamataEngine::Model2* model);

	void Update();

	void Draw(KamataEngine::Camera& camera);

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_;
};
