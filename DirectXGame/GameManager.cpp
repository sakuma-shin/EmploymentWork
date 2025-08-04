#include "GameManager.h"


GameManager::GameManager() {
	dxCommon = DirectXCommon::GetInstance();

	// DirectXCommonクラスが管理している、ウインドウの幅と高さの取得
	 w = dxCommon->GetBackBufferWidth();
	 h = dxCommon->GetBackBufferHeight();


	 input_ = Input::GetInstance();
	 input_->Initialize();

	// 各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<GameScene>();
	sceneArr_[RESULT] = std::make_unique<ResultScene>();

	currentSceneNo_ = TITLE;

	// ゲーム開始時に最初にロードされるシーンのInitializeをここで呼び出す
	sceneArr_[currentSceneNo_]->Initialize();
}

GameManager::~GameManager() {}

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
