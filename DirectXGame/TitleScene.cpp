#include "TitleScene.h"
using namespace KamataEngine;

void TitleScene::Initialize() {
	Model2::StaticInitialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {30.0f, 30.0f, 30.0f};
	worldTransform_.translation_ = {-10.0f, -8.0f, 0.0f};
	worldTransform_.UpdateMatrix();

	camera_.Initialize();

	bgGH = TextureManager::Load("titleScene.png");
	titleFontGH = TextureManager::Load("titleFont.png");
	hitFontGH = TextureManager::Load("titleFont2.png");

	bgSprite_ = Sprite::Create(bgGH, {0, 0});
	titleFontSprite_ = Sprite::Create(titleFontGH, {0, 120.0f});
	hitFontSprite_ = Sprite::Create(hitFontGH, {0, 600.0f});

	bookClosedModel_ = Model2::CreateFromOBJ("book1_closed");
	bookOpenModel_ = Model2::CreateFromOBJ("book1_Open");
	OpenBookTextureHandle_ = TextureManager::Load("Book1_Open.png");

	fontTimer_ = 0;

	moveTimer_ = 0.0f;

	startPos_ = -200.0f;
	endPos_ = 120.0f;

	input_=Input::GetInstance();
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNo = STAGE;
	}
	
	
	if (moveTimer_<kMaxTimer) {
		moveTimer_++;
	} else {
		fontTimer_++;
	}

	titleFontSprite_->SetPosition({0.0f, startPos_ + (endPos_ - startPos_) * EaseOutBounce(moveTimer_/kMaxTimer)});

}

void TitleScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());

	bgSprite_->Draw();
	titleFontSprite_->Draw();
	if (fontTimer_ % 60 >= 30) {
		hitFontSprite_->Draw();
	}

	Sprite::PostDraw();

	dxCommon->ClearDepthBuffer();

	Model2::PreDraw(dxCommon->GetCommandList());

	bookOpenModel_->Draw(worldTransform_, camera_,OpenBookTextureHandle_);

	Model2::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	Sprite::PostDraw();
}

TitleScene::~TitleScene() {
	Model2::StaticFinalize(); 
	delete bgSprite_;
	delete titleFontSprite_;
	delete hitFontSprite_;
}
