#include "GameScene.h"
#include "kamataEngine.h"
#include"GameManager.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE3C_10_サクマ_シン");

	GameManager* gameManager = new GameManager();

	gameManager->Run();

	delete gameManager;
	// エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}