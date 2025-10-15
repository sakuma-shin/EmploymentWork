#include "GameScene.h"
#include<fstream>
using namespace KamataEngine;

GameScene::~GameScene() {
	Model2::StaticFinalize();
	delete player_;
	delete railCamera_;
	delete playerModel_;
	delete skyDomeModel_;
	delete skyDome_;
	delete debugCamera_;
	delete enemyModel_;
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

	LoadEnemyPopData();

}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_RETURN)) {

		sceneNo = RESULT;
	}

	// 敵の更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	// デスフラグが立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	//プレイヤーアップデート
	player_->Update();

	UpdateEnemyPopCommands();

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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/EnemyData/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isSpawn_) {
		spawnTimer_--;
		if (spawnTimer_ <= 0) {
			// 待機完了
			isSpawn_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 一行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭の文字列を取得
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			SpawnEnemy({x, y, z});
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isSpawn_ = true;
			spawnTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::SpawnEnemy(Vector3 spawnPos) {
	// 敵キャラ関連
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(enemyModel_, enemyTextureHandle_, spawnPos);

	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);

	enemies_.push_back(newEnemy);
}