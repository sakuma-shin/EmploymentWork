#pragma once
#include "KamataEngine.h"
#include"Model2.h"
#include"PlayerBullet.h"
#include<list>
class Player {
public:

	void Initialize(KamataEngine::Model2 *model,KamataEngine::Vector3 position);

	void Update();

	void Draw(KamataEngine::Camera &camera);

	~Player();

	//親となるワールドトランスフォームをセット
	void SetParent(const KamataEngine::WorldTransform* parent);

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

private:

	KamataEngine::Input *input_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model2 *model_ = nullptr;
	
	std::list<PlayerBullet*> bullets_;

};
