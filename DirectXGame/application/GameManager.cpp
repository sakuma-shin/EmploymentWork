#include "GameManager.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "TitleScene.h"

GameManager::GameManager() {
	dxCommon = DirectXCommon::GetInstance();

	// DirectXCommonクラスが管理している、ウインドウの幅と高さの取得
	w = dxCommon->GetBackBufferWidth();
	h = dxCommon->GetBackBufferHeight();

	input_ = Input::GetInstance();
	input_->Initialize();

	currentSceneNo_ = TITLE;
	prevSceneNo_ = TITLE;

	// ゲーム開始時に最初にロードされるシーンのInitializeをここで呼び出す
	scene_ = std::make_unique<TitleScene>();
	scene_->Initialize();
}

GameManager::~GameManager() {}

int GameManager::Run() {
	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = scene_->GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			// 新しいシーンを生成する
			switch (currentSceneNo_) {
			case TITLE:
				scene_ = std::make_unique<TitleScene>();
				break;
			case STAGE:
				scene_ = std::make_unique<GameScene>();
				break;
			case RESULT:
				scene_ = std::make_unique<ResultScene>();
				break;
			}

			// 新しいシーンを初期化
			scene_->Initialize();
		}

		// 更新処理
		scene_->Update();

		// 描画開始
		dxCommon->PreDraw();

		// 描画処理
		scene_->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}
	return 0;
}
