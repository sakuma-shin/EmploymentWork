#include "GameScene.h"
#include <fstream>
using namespace KamataEngine;
using namespace MathUtility;

GameScene::~GameScene() {
	Model2::StaticFinalize();
}

void GameScene::Initialize() {

	Model2::StaticInitialize();

	worldTransform_.Initialize();

	camera_.Initialize();

	input_ = Input::GetInstance();

	Vector3 railCameraDistance = {0.0f, -7.0f, 28.0f};
	Vector3 playerPosition = {0.0f, -4.0f, 28.0f};
	Vector3 railCameraPos = playerPosition - railCameraDistance;
	Vector3 startPos = railCameraPos;
	startPos.z = 200.0f;
	Vector3 railCameraRot = {0.1f, 0.0f, 0.0f};

	/*レールカメラ初期化*/
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(startPos, railCameraPos, railCameraRot);

	/*プレイヤー関連初期化*/
	player_ = std::make_unique<Player>();
	playerModel_ = Model2::CreateFromOBJ("player");

	player_->Initialize(playerModel_.get(), playerPosition, railCameraRot);

	playerBulletModel_ = Model2::CreateFromOBJ("playerBullet");

	EnemyBulletModel_ = Model2::CreateFromOBJ("enemyBullet");

	// プレイヤーとレールカメラの親子付け
	player_->SetParent(&railCamera_->GetWorldTransform());

	skyDomeModel_ = Model2::CreateFromOBJ("skyCube");
	skyDomes_.resize(domeNum);
	const float distance = 30.0f;
	/*天球*/
	for (int i = 0; i < skyDomes_.size(); i++) {
		skyDomes_[i] = std::make_unique<SkyDome>();
		float posZ = distance * i;
		skyDomes_[i]->Initialize(skyDomeModel_.get(), posZ);
	}

	// ドア
	doorModel_ = Model2::CreateFromOBJ("Door");

	door_ = std::make_unique<Door>();

	Vector3 doorPos = {0.0f, 0.0f, distance * float(domeNum) - distance / 2.0f};
	door_->Initialize(doorModel_.get(), doorPos);

	// 敵キャラ関連
	enemyModel0_ = Model2::CreateFromOBJ("enemy0");
	enemyModel1_ = Model2::CreateFromOBJ("enemy1");
	enemyModel2_ = Model2::CreateFromOBJ("enemy2");
	enemyTextureHandle_ = TextureManager::Load("enemy.png");

	startTextureHandle_ = TextureManager::Load("startFont.png");
	startSprite_.reset(Sprite::Create(startTextureHandle_, {0, 296.0f}));

	wallModel_ = Model2::CreateFromOBJ("bookshelf");
	wallTextureHandle_ = TextureManager::Load("bookshelf.png");

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	LoadEnemyPopData();

	phase_ = Phase::START;

	// クリア演出
	clearTimer_ = 120;
	clearTextureHandle_ = TextureManager::Load("white1x1.png");
	isClear = false;
	clearSprite_.reset(Sprite::Create(clearTextureHandle_, {}));

	lifeTextureHandle_ = TextureManager::Load("life.png");

	std::vector<Vector2> lifePos;
	lifePos.resize(player_->kMaxLife);
	lifeSprites_.resize(player_->kMaxLife);

	Vector2 lifeSize = {64.0f, 64.0f};

	for (int i = 0; i < lifeSprites_.size(); i++) {
		lifePos[i] = {36.0f, 36.0f + i * 64.0f};

		lifeSprites_[i].reset(Sprite::Create(lifeTextureHandle_, lifePos[i]));
		lifeSprites_[i]->SetTextureRect({}, {64.0f, 64.0f});
		lifeSprites_[i]->SetSize(lifeSize);
	}
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

	door_->Draw(camera_);
	for (int i = 0; i < skyDomes_.size(); i++) {
		skyDomes_[i]->Draw(camera_);
	}

	player_->Draw(camera_);

	for (auto& enemyPtr : enemies_) {
		enemyPtr->Draw(camera_);
	}

	for (auto& bulletPtr : enemyBullets_) {
		bulletPtr->Draw(camera_);
	}

	for (auto& wallPtr : walls_) {
		wallPtr->Draw(camera_);
	}

	Model2::PostDraw();

	Sprite::PreDraw(dxCommon->GetCommandList());

	if (phase_ == Phase::START) {
		if (railCamera_->GetTimer() >= 90 && railCamera_->GetTimer() <= 120) {
			startSprite_->Draw();
		}
	}

	for (auto& spritePtr : lifeSprites_) {
		spritePtr->Draw();
	}

	if (isClear) {
		clearSprite_->Draw();
	}

	Sprite::PostDraw();
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData() {
	enemyPopCommands.clear();
	enemyPopCommands.str("");

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

	float playerPosZ = player_->GetWorldPosition().z;

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
		if (word.find("ENEMY") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// Z座標
			getline(line_stream, word, ',');
			float z = playerPosZ + (float)std::atof(word.c_str());

			SpawnEnemy({x, y, z});
		} else if (word.find("WALL") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// Y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			/*float y = -30.0f;*/

			// Z座標
			getline(line_stream, word, ',');
			float z = playerPosZ + (float)std::atof(word.c_str());

			// Scale X
			getline(line_stream, word, ',');
			float sx = (float)std::atof(word.c_str());

			// Scale Y
			getline(line_stream, word, ',');
			float sy = (float)std::atof(word.c_str());

			// Scale Z
			getline(line_stream, word, ',');
			float sz = (float)std::atof(word.c_str());

			SpawnWall({x, y, z}, {sx, sy, sz}); // Wall生成関数を呼び出す
		} else if (word.find("REPEAT_ENEMY") == 0) {
			getline(line_stream, word, ',');
			// 並べる個数を取得
			int32_t count = std::atoi(word.c_str());

			getline(line_stream, word, ',');
			// 戦闘の敵のX座標を取得
			float startX = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			// 先頭のY座標
			float startY = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			// 先頭のZ座標
			float startZ = playerPosZ + (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			// X座標の間隔
			float offsetX = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			// Y座標の間隔
			float offsetY = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			// Z座標の間隔
			float offsetZ = (float)std::atof(word.c_str());

			// 敵の数だけ敵を生成
			for (int i = 0; i < count; i++) {
				Vector3 pos;

				pos.x = startX + (offsetX * i);

				pos.y = startY + (offsetY * i);

				pos.z = startZ + (offsetZ * i);

				SpawnEnemy(pos);
			}

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

	// 敵生成時
	std::vector<KamataEngine::Model2*> enemyModels;
	enemyModels.push_back(enemyModel0_.get());
	enemyModels.push_back(enemyModel1_.get());
	enemyModels.push_back(enemyModel2_.get());
	// 敵キャラ初期化
	auto newEnemy = std::make_unique<Enemy>();
	newEnemy->Initialize(enemyModels, enemyTextureHandle_, spawnPos, EnemyBulletModel_.get());

	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);

	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_.get());

	enemies_.push_back(std::move(newEnemy));
}

void GameScene::SpawnWall(KamataEngine::Vector3 spawnPos, KamataEngine::Vector3 spawnScale) {
	auto newWall = std::make_unique<Wall>();
	newWall->Initialize(wallModel_.get(), spawnPos, spawnScale, wallTextureHandle_);
	walls_.push_back(std::move(newWall));
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	float radiusA, radiusB;

	// AABB用のサイズ変数
	Vector3 sizeA, sizeB;

	// 自弾リストの取得
	const auto& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const auto& enemyBullets = GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの大きさを取得
	sizeA = player_->GetSize();

	// 自キャラと敵弾全ての当たり判定
	for (const auto& bulletPtr : enemyBullets) {
		EnemyBullet* bullet = bulletPtr.get();
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 敵弾の半径
		radiusB = bullet->GetRadius();

		// AABBとSphereの衝突判定
		if (IsCollisionAABBtoSphere(posA, sizeA, posB, radiusB)) {
			// 敵弾の衝突時のコールバック関数を呼び出す
			bullet->OnCollision();
			// 自キャラの衝突時コールバック関数を呼び出す
			if (!player_->IsDamaged()) {
				player_->OnCollision();
				railCamera_->RequestShake(0.8f);
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵のワールド座標を取得
	for (auto& enemyPtr : enemies_) {
		Enemy* enemy = enemyPtr.get();
		if (enemy->GetPhase() == Enemy::Phase::Approach) {
			posA = enemy->GetWorldPosition();
			// 敵キャラの半径を取得
			sizeA = enemy->GetSize();

			// 自キャラと敵弾全ての当たり判定
			for (const auto& bulletPtr : playerBullets) {
				PlayerBullet* bullet = bulletPtr.get();
				// 自弾の座標
				posB = bullet->GetWorldPosition();

				// 自弾の半径
				radiusB = bullet->GetRadius();

				// AABBとSphereの衝突判定
				if (IsCollisionAABBtoSphere(posA, sizeA, posB, radiusB)) {
					// 敵キャラの衝突時のコールバック関数を呼び出す
					enemy->OnCollision();
					// 自弾の衝突時コールバック関数を呼び出す
					bullet->OnCollision();

					player_->GenerateParticle();
				}
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 自キャラと敵弾全ての当たり判定
	for (const auto& playerBulletPtr : playerBullets) {
		PlayerBullet* playerBullet = playerBulletPtr.get();
		for (const auto& enemyBulletPtr : enemyBullets) {
			EnemyBullet* enemyBullet = enemyBulletPtr.get();
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

				player_->GenerateParticle();
			}
		}
	}
#pragma endregion

#pragma region 自キャラと敵キャラの当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径を取得
	sizeA = player_->GetSize();

	for (const auto& enemyPtr : enemies_) {
		Enemy* enemy = enemyPtr.get();
		if (enemy->GetPhase() == Enemy::Phase::Approach) {
			// 自キャラの座標
			posB = enemy->GetWorldPosition();

			// 自キャラの半径を取得
			sizeB = enemy->GetSize();

			// AABBとAABBの衝突判定
			if (IsCollisionAABBtoAABB(posA, sizeA, posB, sizeB)) {
				/*enemy->OnCollision();*/
				// 自キャラの衝突時コールバック関数を呼び出す
				if (!player_->IsDamaged()) {
					player_->OnCollision();
					railCamera_->RequestShake(0.8f);
				}
			}
		}
	}

#pragma region 自キャラと壁の当たり判定

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラの半径を取得
	sizeA = player_->GetSize();

	for (const auto& wallPtr : walls_) {
		Wall* wall = wallPtr.get();
		// 壁の座標
		posB = wall->GetWorldPosition();

		// 壁の半径を取得
		sizeB = wall->GetSize();

		// 球と球の衝突判定
		if (IsCollisionAABBtoAABB(posA, sizeA, posB, sizeB)) {
			// 壁の衝突時のコールバック関数を呼び出す
			/*wall->OnCollision();*/

			// 自キャラの衝突時コールバック関数を呼び出す
			if (!player_->IsDamaged()) {
				player_->OnCollision();
				railCamera_->RequestShake(0.8f);
			}
		}
	}
#pragma endregion

#pragma region 自弾と壁の当たり判定
	for (const auto& playerBulletPtr : playerBullets) {
		PlayerBullet* playerBullet = playerBulletPtr.get();
		posA = playerBullet->GetWorldPosition();
		radiusA = playerBullet->GetRadius();

		for (const auto& wallPtr : walls_) {
			// 壁の座標
			posB = wallPtr->GetWorldPosition();

			// 壁の半径を取得
			sizeB = wallPtr->GetSize();

			// 球と球の衝突判定
			if (IsCollisionAABBtoSphere(posB, sizeB, posA, radiusA)) {
				playerBullet->OnCollision();

				player_->GenerateParticle();
			}
		}
	}
#pragma endregion
}

void GameScene::StartDirection() {

	railCamera_->StartDirection();
	camera_.matView = railCamera_->GetCamera().matView;
	camera_.matProjection = railCamera_->GetCamera().matProjection;
	// カメラ行列の転送
	camera_.TransferMatrix();

	if (railCamera_->IsFinishedStartDirection()) {
		phase_ = Phase::PLAY;
		Sleep(200);
	}
}

void GameScene::PlayUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneNo = RESULT;
	}

	if (skyDomes_[domeNum - 1]->GetWorldPosition().z <= player_->GetWorldPosition().z) {
		isClear = true;
	}

	if (player_->IsDead()) {

		sceneNo = RESULT;
	}

	if (isClear) {

		clearTimer_--;
		clearSprite_->SetSize({1280.0f, 720.0f});

		Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f - float(clearTimer_) / 120.0f};
		clearSprite_->SetColor(color);

		if (clearTimer_ <= 0.0f) {
			sceneNo = RESULT;
		}
	} else {

		if (player_->GetState() == Player::State::kPlay) {
			// レールカメラ更新
			railCamera_->Update();
		}
	// プレイヤーアップデート
	player_->Update(playerBulletModel_.get());

		camera_.matView = railCamera_->GetCamera().matView;
		camera_.matProjection = railCamera_->GetCamera().matProjection;
		// カメラ行列の転送
		camera_.TransferMatrix();
	}

	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		return enemy->IsDead();
	});

	Vector3 playerPos = player_->GetWorldPosition();
	// 敵の更新
	for (const auto& enemyPtr : enemies_) {
		Enemy* enemy = enemyPtr.get();
		enemy->Update();

		Vector3 posB = enemy->GetWorldPosition();

		if (playerPos.z >= posB.z + 20.0f) {
			enemy->Dead();
		}
	}
	// デスフラグが立った弾を削除
	enemyBullets_.remove_if([](const std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	// 弾の更新
	for (auto& bullet : enemyBullets_) {
		bullet->Update();
	}

	UpdateEnemyPopCommands();

	// 壁のデスフラグ処理を追加
	walls_.remove_if([](const std::unique_ptr<Wall>& wall) {
		return wall->IsDead();
	});

	// 壁の更新
	for (auto& wallPtr : walls_) {
		wallPtr->Update();
		Vector3 posB = wallPtr->GetWorldPosition();

		if (playerPos.z >= posB.z + 20.0f) {
			wallPtr->OnCollision();
		}
	}

	int life = player_->GetLife();
	int maxLife = player_->kMaxLife;
	int lostLife = maxLife - life;

	for (int i = 0; i < lifeSprites_.size(); i++) {
		if (i < lostLife) {
			lifeSprites_[i]->SetTextureRect({64.0f, 0.0f}, {64.0f, 64.0f});
		} else {
			lifeSprites_[i]->SetTextureRect({0.0f, 0.0f}, {64.0f, 64.0f});
		}
	}

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
