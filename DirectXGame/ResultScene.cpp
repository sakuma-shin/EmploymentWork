#include "ResultScene.h"
using namespace KamataEngine;

void ResultScene::Initialize() {
	Model2::StaticInitialize();

	worldTransform_.Initialize();
	
	camera_.Initialize();

	input_ = Input::GetInstance();
}

void ResultScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNo = TITLE;
	}
}

void ResultScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	Sprite::PostDraw();

	dxCommon->ClearDepthBuffer();

	Model2::PreDraw(dxCommon->GetCommandList());

	Model2::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	Sprite::PostDraw();
}

ResultScene::~ResultScene() { Model2::StaticFinalize(); }


