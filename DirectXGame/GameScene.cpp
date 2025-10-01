#include "GameScene.h"
using namespace KamataEngine;

GameScene::~GameScene() {
	Model2::StaticFinalize();
	delete player_;
	delete railCamera_;
	delete playerModel_;
	delete skyDomeModel_;
	delete skyDome_;
	delete debugCamera_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
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

	/*天球*/
	skyDome_ = new SkyDome();
	skyDomeModel_ = Model2::CreateFromOBJ("sphere");

	skyDome_->Initialize(skyDomeModel_);
	
		// 敵キャラ関連
	enemyModel_ = Model2::CreateFromOBJ("enemy");
	enemyTextureHandle_ = TextureManager::Load("enemy.png");

	Enemy* newEnemy = new Enemy();
	// 初期化
	Vector3 enemyPosition = {4.0f, 0.0f, 80.0f};
	newEnemy->Initialize(enemyModel_, enemyTextureHandle_, enemyPosition);

	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);

	enemies_.push_back(newEnemy);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);


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


	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	}
	
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
	skyDome_->Draw(camera_);

	player_->Draw(camera_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}

	Model2::PostDraw();

	//前景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 前景スプライト描画後処理
	Sprite::PostDraw();
}
