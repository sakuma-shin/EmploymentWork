#pragma once
#include"Particle.h"
#include"KamataEngine.h"
#include"Model2.h"

class PlayerBulletParticle {
public:
	// 全パーティクル生成（爆発的な発生）
	void Initialize(KamataEngine::Model2* particleModel, const KamataEngine::Vector3& emitterPosition);

	// 更新
	void Update();

	// 描画
	void Draw(KamataEngine::Camera& camera);

	// 全てのパーティクルが死亡したか
	bool IsDead() const { return particles_.empty(); }

	private:
	std::list<Particle*> particles_;
	KamataEngine::Model2* model_ = nullptr;

	// ランダム生成用
	std::mt19937 randomEngine_;
	std::uniform_real_distribution<float> distribution_{-1.0f, 1.0f};
};