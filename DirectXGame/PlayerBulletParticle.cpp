#include "PlayerBulletParticle.h"

using namespace KamataEngine;
using namespace MathUtility;
/// <summary>
/// 発生
/// </summary>
/// <param name="particleModel"></param>
/// <param name="emitterPosition"></param>
void PlayerBulletParticle::Emit(KamataEngine::Model2* particleModel, const KamataEngine::Vector3& emitterPosition) {

	model_ = particleModel;

	particles_.clear(); // 既にパーティクルが存在する場合はクリア

	// 発生させるパーティクルの数
	const int kNumParticles = 20;

	// 速度の最大値
	const float kMaxSpeed = 0.3f;
	// 寿命の範囲
	const float kMinLife = 30.0f;
	const float kMaxLife = 60.0f;

	// 乱数エンジンのシードを初期化
	randomEngine_.seed(static_cast<unsigned int>(std::time(nullptr)));

	for (int i = 0; i < kNumParticles; ++i) {
		// ランダムな方向と速度を生成（飛び散る表現）
		float x = distribution_(randomEngine_);
		float y = distribution_(randomEngine_);
		float z = distribution_(randomEngine_);

		// ベクトルを正規化してランダムな速さをかける
		Vector3 randomVelocity = {x, y, z};
		float length = std::sqrt(randomVelocity.x * randomVelocity.x + randomVelocity.y * randomVelocity.y + randomVelocity.z * randomVelocity.z);
		if (length > 0) {
			randomVelocity.x /= length;
			randomVelocity.y /= length;
			randomVelocity.z /= length;
		}

		// 速度を適用
		float speed = distribution_(randomEngine_) * 0.5f * kMaxSpeed + 0.5f * kMaxSpeed; // [0.5 * kMaxSpeed, 1.0 * kMaxSpeed] の範囲
		randomVelocity.x *= speed;
		randomVelocity.y *= speed;
		randomVelocity.z *= speed;

		// 寿命
		float lifeTime = distribution_(randomEngine_) * 0.5f * (kMaxLife - kMinLife) + 0.5f * (kMaxLife + kMinLife);

		// パーティクルを生成し、リストに追加
		Particle* newParticle = new Particle();
		newParticle->Initialize(emitterPosition, randomVelocity, lifeTime);

		particles_.push_back(newParticle);
	}
}

void PlayerBulletParticle::Update() {
	// 死亡したパーティクルを削除
	particles_.remove_if([](Particle* particle) { return particle->IsDead(); });

	// パーティクルの更新
	for (Particle* particle : particles_) {
		particle->Update();
	}
}

void PlayerBulletParticle::Draw(Camera& camera) {
	// パーティクルごとの Draw を実行
	for (Particle* particle : particles_) {
		particle->Draw(camera);
	}
}
