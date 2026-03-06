#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "BaseObject.h"

class SkyDome : public BaseObject {
public:
	void Initialize(KamataEngine::Model2* model, float positionZ);
	void Update() override;
	void Draw(KamataEngine::Camera& camera) override;
	KamataEngine::Vector3 GetWorldPosition();
private:

};
