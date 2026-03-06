#pragma once
#include "BaseObject.h"
#include "IScene.h"
#include "KamataEngine.h"
#include "Model2.h"
#include <cassert>
#include <list>
#include <vector>

class Player;
class GameScene;

class Enemy : public BaseObject {
public:
	Enemy();
	~Enemy();

	void Initialize(std::vector<KamataEngine::Model2*>& models, uint32_t textureHandle, const KamataEngine::Vector3& position, KamataEngine::Model2* bulletModel);

	void Update() override;

	void Draw(KamataEngine::Camera& camera) override;

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	void Approach();
	void Leave();

	enum class Phase {
		Approach,
		Leave,
	};

	void Fire();

	void PhaseInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	KamataEngine::Vector3 GetWorldPosition();

	static const int kFireInterval = 180;
	static const int kAnimationInterval = 10;

	KamataEngine::Vector3 GetSize() { return size_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

	Phase GetPhase() const { return phase_; }

	void Dead();

private:
	// worldTransform_ / color_ は BaseObject 側
	std::vector<KamataEngine::Model2*> models_;
	int32_t currentModelIndex_ = 0;
	int32_t animationTimer_ = 0;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Vector3 position_ = {};
	KamataEngine::Vector3 velocity_ = {};
	Phase phase_ = Phase::Approach;
	static const int kLeaveDuration = 60;
	int32_t leaveTimer_ = 0;
	const int32_t kMaxLeaveTimer_ = 60;
	KamataEngine::Vector3 initialTranslation_ = {};
	float flyUpHeight_ = 1.5f;
	int32_t fireTimer_ = 0;
	Player* player_ = nullptr;
	KamataEngine::Vector3 size_ = {};
	GameScene* gameScene_ = nullptr;
	bool isDead_ = false;
	KamataEngine::Model2* bulletModel_ = nullptr;
};