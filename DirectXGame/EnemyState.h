#pragma once

class Enemy; // 前方宣言

// 敵の状態のインターフェース
class IEnemyState {
public:
	virtual ~IEnemyState() = default;
	virtual void Update(Enemy* enemy) = 0;
};

// 接近フェーズ
class EnemyStateApproach : public IEnemyState {
public:
	void Update(Enemy* enemy) override;
};

// 離脱フェーズ
class EnemyStateLeave : public IEnemyState {
public:
	void Update(Enemy* enemy) override;
};
