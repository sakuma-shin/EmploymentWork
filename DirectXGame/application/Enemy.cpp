#include "Enemy.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "Player.h"
#include "Easing.h"

using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(KamataEngine::Model2* model, uint32_t textureHandle, const KamataEngine::Vector3& position, KamataEngine::Model2* bulletModel) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	bulletModel_ = bulletModel;

	textureHandle_ = textureHandle;

	// ワ－ルドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数からpositionを代入して初期座標を設定
	worldTransform_.translation_ = position;
	// 敵が正面を向くようにする
	worldTransform_.rotation_.y = 3.2f;

	PhaseInitialize();

	size_ = {5.0f, 4.0f, 1.0f};

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

void Enemy::Draw(Camera& camera) { model_->Draw(worldTransform_, camera, textureHandle_); }

void Enemy::OnCollision() { phase_ = Phase::Leave; }

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

void Enemy::Leave() {
	if (leaveTimer_ > 0) {
		// タイマーを減らす
		leaveTimer_--;

		// 0.0fから1.0fまでの進行度を計算
		float t = 1.0f - (float)leaveTimer_ / kLeaveDuration;

		// 死亡演出のY軸移動を計算
		float y_offset;

		if (t <= 0.5f) { // 最初の半分で上昇
			// tを0.0f〜1.0fの範囲に正規化し、EaseOutQuadで緩やかに上昇させる
			float t_up = t * 2.0f;
			float ease_t = EaseOutQuad(t_up); // EaseOutQuad(t) = 1 - (1 - t) * (1 - t);
			y_offset = ease_t * flyUpHeight_;
		} else { // 残りの半分で落下
			// tを0.0f〜1.0fの範囲に正規化し、EaseInQuadで落下させる
			float t_down = (t - 0.5f) * 2.0f;
			float ease_t = EaseInQuad(t_down); // EaseInQuad(t) = t * t;

			// 上昇分の高さから、落下分を引く
			// 落下開始時の位置が最大浮き上がり位置になる
			y_offset = flyUpHeight_ - (ease_t * (initialTranslation_.y + flyUpHeight_)); // Y軸の初期値まで落下させる

			// 最終的にY軸が initialTranslation_.y の位置より下がるように、落下距離を調整します。
			// ここではシンプルに、最大浮き上がり位置から「元の位置より少し下」まで落下させるように調整します。
			float dropDistance = initialTranslation_.y + flyUpHeight_ + 2.0f; // 元の位置より2.0f下まで
			y_offset = flyUpHeight_ - (ease_t * dropDistance);
		}

		// 座標の更新（XとZは固定、Yだけを演出で動かす）
		worldTransform_.translation_.y = initialTranslation_.y + y_offset;

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
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, BulletVelocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
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
