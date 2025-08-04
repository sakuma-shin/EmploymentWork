#include "GameScene.h"
using namespace KamataEngine;

GameScene::~GameScene() {
	Model2::StaticFinalize();
	delete player_;
	delete railCamera_;
	delete playerModel_;
}

void GameScene::Initialize() {

	Model2::StaticInitialize();

	worldTransform_.Initialize();

	camera_.Initialize();

	input_ = Input::GetInstance();

	
	/*プレイヤー関連初期化*/
	player_ = new Player();
	playerModel_ = Model2::CreateFromOBJ("player");
	Vector3 playerPosition = Vector3(0.0f, 0.0f,20.0f);
	player_->Initialize(playerModel_,playerPosition);

	/*レールカメラ初期化*/
	railCamera_ = new RailCamera();
	Vector3 railCameraPos = playerPosition;
	railCameraPos.z -= 25.0f;
	Vector3 railCameraRot = {0.0f, 0.0f, 0.0f};
	railCamera_->Initialize(railCameraPos,railCameraRot);

	//プレイヤーとレールカメラの親子付け
	player_->SetParent(&railCamera_->GetWorldTransform());

}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_RETURN)) {

		sceneNo = RESULT;
	}

	//プレイヤーアップデート
	player_->Update();

	//レールカメラ更新
	railCamera_->Update();
	camera_.matView = railCamera_->GetCamera().matView;
	camera_.matProjection = railCamera_->GetCamera().matProjection;
	// カメラ行列の転送
	camera_.TransferMatrix();
	
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//後景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());


	//後景スプライト描画後処理
	Sprite::PostDraw();

	//深度クリア
	dxCommon->ClearDepthBuffer();


	Model2::PreDraw(dxCommon->GetCommandList());

	player_->Draw(camera_);

	Model2::PostDraw();

	//前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 前景スプライト描画後処理
	Sprite::PostDraw();
}
