#include "PlayerState.h"
#include "Player.h"

void PlayerStatePlay::Update(Player* player, KamataEngine::Model2* model) { player->PlayUpdate(model); }
void PlayerStateDeathRotate::Update(Player* player, KamataEngine::Model2* model) { player->DeathRotate(); }
void PlayerStateDeathDrop::Update(Player* player, KamataEngine::Model2* model) { player->DeathDrop(); }
void PlayerStateDeathDisappear::Update(Player* player, KamataEngine::Model2* model) { player->DeathDisappear(); }