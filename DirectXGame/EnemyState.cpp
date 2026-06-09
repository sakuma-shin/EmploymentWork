#include "EnemyState.h"
#include "Enemy.h"

void EnemyStateApproach::Update(Enemy* enemy) {
	enemy->Approach(); // 実際の処理はEnemy側に残すか、State側に移動させます
}

void EnemyStateLeave::Update(Enemy* enemy) { enemy->Leave(); }
