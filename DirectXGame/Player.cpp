#include "Player.h"
#include <cassert>

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model2* model, KamataEngine::Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	input_ = Input::GetInstance();
}

void Player::Update() {
	const float kSpeed = 0.5f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= kSpeed;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += kSpeed;
	}

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += kSpeed;
	}

	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= kSpeed;
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(KamataEngine::Camera& camera) { model_->Draw(worldTransform_, camera); }

Player::~Player() {}

void Player::SetParent(const KamataEngine::WorldTransform* parent) {
//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
