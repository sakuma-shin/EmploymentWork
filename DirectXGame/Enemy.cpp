#include "Enemy.h"
#include"Player.h"
#include"GameScene.h"
#include "EnemyBullet.h"

using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(KamataEngine::Model2* model, uint32_t textureHandle, const KamataEngine::Vector3& position) { 
	// NULLポインタチェック
	assert(model);
	model_ = model;

	textureHandle_ = textureHandle;

	// ワ－ルドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数からpositionを代入して初期座標を設定
	worldTransform_.translation_ = position;
	// 敵が正面を向くようにする
	worldTransform_.rotation_.y = 3.2f;

	PhaseInitialize();

	radius_ = 1.5f;

	worldTransform_.UpdateMatrix();

	
}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:
	default:

		Approach();

		break;
	}

	if (worldTransform_.translation_.z <= -0.5f) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera) { model_->Draw(worldTransform_, camera, textureHandle_); }


void Enemy::OnCollision() { isDead_ = true; }

void Enemy::Approach() {
	// 速度の初期化
	const float kSpeed = -0.05f;
	velocity_ = {0.0f, 0.0f, kSpeed};

	// 敵の移動
	worldTransform_.translation_ += velocity_;

	// 発射タイマーカウントダウン
	fireTimer_--;
	//
	if (fireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度を設定
	const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPosition();

	Vector3 enemyPos = GetWorldPosition();

	Vector3 BulletVelocity = playerPos - enemyPos;

	BulletVelocity = Normalize(BulletVelocity);

	BulletVelocity *= kBulletSpeed;
	
	// 速度ベクトルを敵の向きに合わせて回転させる
	BulletVelocity = TransformNormal(BulletVelocity, worldTransform_.matWorld_);

	// 弾を生成し初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, -BulletVelocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::PhaseInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
