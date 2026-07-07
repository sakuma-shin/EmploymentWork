#include "GameScene.h"
#include "kamataEngine.h"
#include"GameManager.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE3C_07_サクマ_シン");

	auto gameManager = std::make_unique<GameManager>();

	gameManager->Run();
	// エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}