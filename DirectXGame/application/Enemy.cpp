#include "Enemy.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "Player.h"
#include "Easing.h"

using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(std::vector<KamataEngine::Model2*>& models, uint32_t textureHandle, const KamataEngine::Vector3& position, KamataEngine::Model2* bulletModel) {
	// NULLポインタチェック
	/*assert(model);*/
	models_ = models;

	bulletModel_ = bulletModel;

	textureHandle_ = textureHandle;

	// ワ－ルドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数からpositionを代入して初期座標を設定
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.x = 3.0f;

	worldTransform_.rotation_.y = 1.6f;

	worldTransform_.scale_ *= 0.7f;

	PhaseInitialize();

	size_ = {4.0f, 4.0f, 1.0f};

	leaveTimer_ = kMaxLeaveTimer_;

	color_.Initialize();

	isDead_ = false;

	worldTransform_.UpdateMatrix();
}

void Enemy::Update() {

	switch (phase_) {
	case Phase::Approach:

		Approach();

		break;

	case Phase::Leave:

		Leave();

		break;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (!models_.empty()) {
		if (phase_ == Phase::Approach) {
			models_[currentModelIndex_]->Draw(worldTransform_, camera, textureHandle_, &color_);
		} else if (phase_ == Phase::Leave) {
			models_[0]->Draw(worldTransform_, camera, textureHandle_, &color_);
		}
	}
}

void Enemy::OnCollision() { phase_ = Phase::Leave; }

void Enemy::Approach() {

	animationTimer_++;
	if (animationTimer_ >= kAnimationInterval) {
		// タイマーをリセット
		animationTimer_ = 0;
		// 次のモデルへインデックスを進める
		currentModelIndex_++;
		// モデルの個数を超えたら0に戻す（ループさせる）
		if (currentModelIndex_ >= models_.size()) {
			currentModelIndex_ = 0;
		}
	}

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

void Enemy::Leave() {
	if (leaveTimer_ > 0) {

		worldTransform_.rotation_.z = -1.5f;

		// タイマーを減らす
		leaveTimer_--;
	
		float downVelocity = -0.4f;

		worldTransform_.translation_.y += downVelocity;

		float alpha = float(leaveTimer_) / float(kMaxLeaveTimer_);

		color_.SetColor({1.0f, 1.0f, 1.0f, alpha});

	} else {
		// 演出終了、死亡させる
		isDead_ = true;
	}
}

void Enemy::Fire() {
	assert(player_);

	//// 弾の速度を設定
	// const float kBulletSpeed = 1.0f;
	Vector3 playerPos = player_->GetWorldPosition();

	Vector3 enemyPos = GetWorldPosition();

	Vector3 BulletVelocity = {0.0f, 0.0f, -0.5f};

	// 弾を生成し初期化
	auto newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, BulletVelocity);

	// 弾を登録する（所有権は GameScene に譲渡する）
	gameScene_->AddEnemyBullet(std::move(newBullet));
}

void Enemy::PhaseInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;

	if (phase_ == Phase::Leave) { 
		leaveTimer_ = kLeaveDuration;
		initialTranslation_ = worldTransform_.translation_; 
	}
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Dead() { isDead_ = true; }
