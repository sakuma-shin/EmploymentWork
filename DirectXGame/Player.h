#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include"PlayerBullet.h"
#include"PlayerBulletParticle.h"
#include<list>
class Player {
public:

	void Initialize(KamataEngine::Model2 *model,KamataEngine::Vector3 position);

	void Update();

	void Draw(KamataEngine::Camera &camera);

	~Player();

	//親となるワールドトランスフォームをセット
	void SetParent(const KamataEngine::WorldTransform* parent);

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 半径のGetter
	float GetRadius() { return radius_; }

	bool IsDead() const { return isDead_; }

private:

	KamataEngine::Input *input_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model2 *model_ = nullptr;
	// 半径
	float radius_ = 0.0f;

	const int kMaxLife = 5;

	int life = 0;

	bool isDead_ = false;
	
	std::list<PlayerBullet*> bullets_;
	std::list<PlayerBulletParticle*> inkParticles_;
};
