#pragma once
#include "KamataEngine.h"
#include"Model2.h"
class PlayerBullet {
public:
	// 初期化
	void Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_;}

private:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model2* model_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};

	float deathTimer_= 60.0f;

	bool isDead_ = false;

};
