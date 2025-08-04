#pragma once
#include "KamataEngine.h"
#include "Player.h"
class PlayerBullet {
	// 初期化
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	// 更新
	void Update();

	// 描画
	void Draw();

	bool IsDead() const { return isDead_;}

private:

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Vector3 velocity_ = {};

	float deathTimer_= 60.0f;

	bool isDead_ = false;

};
