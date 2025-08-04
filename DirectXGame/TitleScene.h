#pragma once
#include "IScene.h"
#include"KamataEngine.h"
#include "Model2.h"
#include"Easing.h"

class TitleScene : public IScene {
public:
	void Initialize()override;

	void Update() override;

	void Draw() override;

	~TitleScene();

private:
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Input* input_;

	uint32_t bgGH = 0u;
	uint32_t titleFontGH = 0u;
	uint32_t hitFontGH = 0u;

	uint32_t fontTimer_ = 0;

	float startPos_ = 0;
	float endPos_ = 0;

	const float kMaxTimer=120.0f;
	float moveTimer_ = 0;

	KamataEngine::Sprite* bgSprite_ = nullptr;
	KamataEngine::Sprite* titleFontSprite_ = nullptr;
	KamataEngine::Sprite* hitFontSprite_ = nullptr;

};
