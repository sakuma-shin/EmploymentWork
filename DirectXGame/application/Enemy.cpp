#include "Enemy.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "Player.h"
#include "Easing.h"

using namespace KamataEngine;
using namespace MathUtility;

Enemy::Enemy() {}

Enemy::~Enemy() {
	delete approachState_;
	delete leaveState_;
}

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

	// 状態のインスタンス生成
	approachState_ = new EnemyStateApproach();
	leaveState_ = new EnemyStateLeave();

	// 初期状態の設定
	state_ = approachState_;
	PhaseInitialize();

	size_ = {4.0f, 4.0f, 1.0f};

	leaveTimer_ = kMaxLeaveTimer_;

	color_.Initialize();

	isDead_ = false;

	worldTransform_.UpdateMatrix();
}

void Enemy::Update() {

	// switch文を削除し、現在の状態クラスにUpdateを委譲する
	if (state_) {
		state_->Update(this);
	}
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(Camera& camera) {
	if (!models_.empty()) {
		if (state_ == approachState_) {
			models_[currentModelIndex_]->Draw(worldTransform_, camera, textureHandle_, &color_);
		} else if (state_ == leaveState_) {
			models_[0]->Draw(worldTransform_, camera, textureHandle_, &color_);
		}
	}
}

void Enemy::OnCollision() { ChangeState(leaveState_); }

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
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, BulletVelocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::PhaseInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;

	if (state_ == leaveState_) { 
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

void Enemy::ChangeState(IEnemyState* newState) {
	state_ = newState;
	PhaseInitialize(); // 状態が切り替わったときの初期化
}

bool Enemy::IsApproachState() const { return state_ == approachState_; }

void Enemy::Dead() { isDead_ = true; }
