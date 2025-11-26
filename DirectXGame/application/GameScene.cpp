#include "GameScene.h"
#include <fstream>
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

	for (EnemyBullet* enemyBullet : enemyBullets_) {
		delete enemyBullet;
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
	Vector3 playerPosition = Vector3(0.0f, 0.0f, 35.0f);
	player_->Initialize(playerModel_, playerPosition);

	/*レールカメラ初期化*/
	railCamera_ = new RailCamera();
	Vector3 railCameraPos = playerPosition;
	railCameraPos.z = 200.0f;
	Vector3 railCameraRot = {0.0f, 0.0f, 0.0f};
	railCamera_->Initialize(railCameraPos, railCameraRot);

	// プレイヤーとレールカメラの親子付け
	player_->SetParent(&railCamera_->GetWorldTransform());
	/*天球*/
	skyDome_ = new SkyDome();
	skyDomeModel_ = Model2::CreateFromOBJ("sphere");

	skyDome_->Initialize(skyDomeModel_);

	// 敵キャラ関連
	enemyModel_ = Model2::CreateFromOBJ("enemy");
	enemyTextureHandle_ = TextureManager::Load("enemy.png");

	startTextureHandle_ = TextureManager::Load("startFont.png");
	startSprite_ = Sprite::Create(startTextureHandle_, {0, 120.0f});

	/*Enemy* newEnemy = new Enemy();*/
	//// 初期化
	// Vector3 enemyPosition = {4.0f, 0.0f, 80.0f};
	// newEnemy->Initialize(enemyModel_, enemyTextureHandle_, enemyPosition);

	//// 敵キャラにゲームシーンを渡す
	// newEnemy->SetGameScene(this);

	//// 敵キャラに自キャラのアドレスを渡す
	// newEnemy->SetPlayer(player_);

	// enemies_.push_back(newEnemy);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	LoadEnemyPopData();

	phase_ = Phase::START;

	//クリア演出
	clearTimer_ = 120;
	clearTextureHandle_ = TextureManager::Load("white1x1.png");
	isClear = false;
	clearSprite_ = Sprite::Create(clearTextureHandle_, {});
}

void GameScene::Update() {
	switch (phase_) {
	case Phase::START:
		StartDirection();

		break;

	case Phase::PLAY:
		PlayUpdate();

		break;
	}
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 後景スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// 後景スプライト描画後処理
	Sprite::PostDraw();

	// 深度クリア
	dxCommon->ClearDepthBuffer();

	Model2::PreDraw(dxCommon->GetCommandList());
	skyDome_->Draw(camera_);

	player_->Draw(camera_);

	for (Enemy* enemy : enemies_) {
		enemy->Draw(camera_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(camera_);
	}

	Model2::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	if (phase_ == Phase::START) {
		if (railCamera_->GetTimer() >= 90 && railCamera_->GetTimer() <= 120) {
			startSprite_->Draw();
		}
	}

	if (isClear) {
		clearSprite_->Draw();
	}

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
	// 敵キャラ初期化
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(enemyModel_, enemyTextureHandle_, spawnPos);

	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);

	enemies_.push_back(newEnemy);
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	float radiusA, radiusB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径を取得
	radiusA = player_->GetRadius();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 敵弾の半径
		radiusB = bullet->GetRadius();

		Vector3 distance;
		distance.x = (posA - posB).x * (posA - posB).x;
		distance.y = (posA - posB).y * (posA - posB).y;
		distance.z = (posA - posB).z * (posA - posB).z;

		// 球と球の衝突判定
		if (distance.x + distance.y + distance.z <= (radiusA + radiusB) * (radiusA + radiusB)) {
			// 自キャラの衝突時のコールバック関数を呼び出す
			player_->OnCollision();

			// 敵弾の衝突時コールバック関数を呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵のワールド座標を取得
	for (Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();
		// 敵キャラの半径を取得
		radiusA = enemy->GetRadius();

		// 自キャラと敵弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 自弾の半径
			radiusB = bullet->GetRadius();

			Vector3 distance;
			distance.x = (posA - posB).x * (posA - posB).x;
			distance.y = (posA - posB).y * (posA - posB).y;
			distance.z = (posA - posB).z * (posA - posB).z;

			// 球と球の衝突判定
			if (distance.x + distance.y + distance.z <= (radiusA + radiusB) * (radiusA + radiusB)) {
				// 自キャラの衝突時のコールバック関数を呼び出す
				enemy->OnCollision();

				// 敵弾の衝突時コールバック関数を呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自キャラと敵弾全ての当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posA = enemyBullet->GetWorldPosition();
			// 自弾の座標
			posB = playerBullet->GetWorldPosition();

			// 敵弾の半径
			radiusA = enemyBullet->GetRadius();
			// 自弾の半径
			radiusB = playerBullet->GetRadius();

			Vector3 distance;
			distance.x = (posA - posB).x * (posA - posB).x;
			distance.y = (posA - posB).y * (posA - posB).y;
			distance.z = (posA - posB).z * (posA - posB).z;

			// 球と球の衝突判定
			if (distance.x + distance.y + distance.z <= (radiusA + radiusB) * (radiusA + radiusB)) {
				// 敵弾の衝突時のコールバック関数を呼び出す
				enemyBullet->OnCollision();

				// 自弾の衝突時コールバック関数を呼び出す
				playerBullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自キャラと敵キャラの当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径を取得
	radiusA = player_->GetRadius();

	for (Enemy* enemy : enemies_) {
		// 自キャラの座標
		posB = enemy->GetWorldPosition();

		// 自キャラの半径を取得
		radiusB = enemy->GetRadius();

		//距離を取得
		Vector3 distance;
		distance.x = (posA.x - posB.x) * (posA.x - posB.x);
		distance.y = (posA.y - posB.y) * (posA.y - posB.y);
		distance.z = (posA.z - posB.z) * (posA.z - posB.z);

		// 球と球の衝突判定
		if (distance.x + distance.y + distance.z <= (radiusA + radiusB) * (radiusA + radiusB)) {
			// 敵弾の衝突時のコールバック関数を呼び出す
			enemy->OnCollision();

			// 自弾の衝突時コールバック関数を呼び出す
			player_->OnCollision();
		}

	}

}

void GameScene::StartDirection() {

	railCamera_->StartDirection();
	camera_.matView = railCamera_->GetCamera().matView;
	camera_.matProjection = railCamera_->GetCamera().matProjection;
	// カメラ行列の転送
	camera_.TransferMatrix();

	if (railCamera_->IsFinishedStartDirection()) {
		phase_ = Phase::PLAY;
	}
}

void GameScene::PlayUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNo = RESULT;
	}

	if (input_->TriggerKey(DIK_C)) {
		isClear=true;
	}

	if (player_->IsDead()) {

		sceneNo = RESULT;
	}

	if (isClear) {

		clearTimer_--;
		clearSprite_->SetSize({1280.0f, 720.0f});

		Vector4 color = {1.0f, 1.0f, 1.0f,1.0f- float(clearTimer_) / 120.0f};
		clearSprite_->SetColor(color);

		if (clearTimer_ <= 0.0f) {
			sceneNo = RESULT;
		}
	}

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

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

	// プレイヤーアップデート
	player_->Update();

	UpdateEnemyPopCommands();

	if (player_->GetState() == Player::State::kPlay) {
		// レールカメラ更新
		railCamera_->Update();
	}
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

	CheckAllCollisions();
}


