#include "GameManager.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"ResultScene.h"


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
	 sceneArr_[currentSceneNo_] = std::make_unique<TitleScene>();
	sceneArr_[currentSceneNo_]->Initialize();
}

GameManager::~GameManager() {  }

int GameManager::Run() {
	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}


		//シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		//シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			// 新しいシーンを生成する
			switch (currentSceneNo_) {
			case TITLE:
				sceneArr_[TITLE] = std::make_unique<TitleScene>();
				break;
			case STAGE:
				sceneArr_[STAGE] = std::make_unique<GameScene>();
				break;
			case RESULT:
				sceneArr_[RESULT] = std::make_unique<ResultScene>();
				break;
			}

			// 新しいシーンを初期化
			sceneArr_[currentSceneNo_]->Initialize();
		}

		//更新処理
		sceneArr_[currentSceneNo_]->Update();

		// 描画開始
		dxCommon->PreDraw();

		//描画処理
		sceneArr_[currentSceneNo_]->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}
	return 0;
}
