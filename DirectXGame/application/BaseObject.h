#pragma once
#include"KamataEngine.h"
#include"Model2.h"
#include <cassert>
class BaseObject {
public:
	virtual void Initialize() = 0;

	virtual void BaseInitialize(KamataEngine::Model2* model, uint32_t textureHandle) {
		assert(model);
		model_ = model;

		worldTransform_.Initialize();

		isDead_ = false;
	}
		

	virtual ~BaseObject();

	//更新処理
	virtual void Update();

	//描画処理
	virtual void Draw(KamataEngine::Camera &camera);

	virtual KamataEngine::Vector3 GetWorldPosition();

	virtual bool IsDead() const { return isDead_; }

protected: 
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_ = nullptr;
	bool isDead_ = false;
	uint32_t textureHandle_ = 0u;
};
