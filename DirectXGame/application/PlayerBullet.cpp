#include "PlayerBullet.h"

using namespace KamataEngine;
using namespace MathUtility;


void PlayerBullet::Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	// NULLポインタチェック

	model_ = model;

	velocity_ = velocity;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	radius_ = 1.0f;

	deathTimer_ = 120.0f;

	color_.Initialize();
	color_.SetColor({0.29f, 0.0f, 0.42f, 1.0f});
}

void PlayerBullet::Update() {

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(Camera& camera) { 
	model_->Draw(worldTransform_, camera,&color_);
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision() { 
	isDead_ = true; 
}
