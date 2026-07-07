#include "EnemyBullet.h"


using namespace KamataEngine;
using namespace MathUtility;

void EnemyBullet::Initialize(KamataEngine::Model2* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	objColor.Initialize();
	objColor.SetColor({0.0f, 0.0f, 0.0f, 1.0f});

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();

	// 引数の座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.UpdateMatrix();

	velocity_ = velocity;

	radius_ = 1.0f;

}

void EnemyBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	// 座標を移動させる
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const KamataEngine::Camera& camera) {
	// モデルの描画
	model_->Draw(worldTransform_, camera, textureHandle_, &objColor);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}