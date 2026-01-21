#pragma once
#include"KamataEngine.h"
#include"Easing.h"

class RailCamera {
public:
	// 初期化
	void Initialize(KamataEngine::Vector3 startPos, KamataEngine::Vector3 endPos,KamataEngine::Vector3 rotate);

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

	// シェイクの開始
	void RequestShake(float intensity) {
		shakeTimer_ = kMaxShakeTimer;
		shakeIntensity_ = intensity;
	}


	void DeathRotate(KamataEngine::Vector3 pos);

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Camera camera_;

	KamataEngine::Input* input_ = nullptr;

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

	// シェイク用変数
	float shakeTimer_ = 0.0f;
	float shakeIntensity_ = 0.0f;
	const float kMaxShakeTimer = 60.0f;

};
