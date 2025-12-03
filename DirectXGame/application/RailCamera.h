#pragma once
#include"KamataEngine.h"
#include"Easing.h"

class RailCamera {
public:
	// 初期化
	void Initialize(KamataEngine::Vector3 pos,KamataEngine::Vector3 rotate);

	// 更新
	void Update();

	//ワールドトランスフォームのゲッター
	KamataEngine::WorldTransform& GetWorldTransform();

	KamataEngine::Camera& GetCamera() { return camera_; }

	/// <summary>
	/// スタート演出
	/// </summary>
	void StartDirection();

	bool IsFinishedStartDirection() const { return timer_ >= kMaxTimer; }

	float GetTimer() const { return timer_; }

	void DeathRotate(KamataEngine::Vector3 pos);

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	/// <summary>
	/// スタート演出のカメラが動くタイマー
	/// </summary>
	float timer_ = 0;
	const float kMoveTimer = 90.0f;
	const float kMaxTimer = 120.0f;

	/// <summary>
	/// スタート演出の始まりと終わりのポジション
	/// </summary>
	KamataEngine::Vector3 startPos_ = {};
	KamataEngine::Vector3 endPos_ = {};

	float deathTimer_ = 0.0f;
	float kMaxDeathTimer = 60.0f;

};
