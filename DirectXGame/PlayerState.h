#pragma once

namespace KamataEngine {
class Model2;
}
class Player;

class IPlayerState {
public:
	virtual ~IPlayerState() = default;
	virtual void Update(Player* player, KamataEngine::Model2* model) = 0;
};

class PlayerStatePlay : public IPlayerState {
public:
	void Update(Player* player, KamataEngine::Model2* model) override;
};

class PlayerStateDeathRotate : public IPlayerState {
public:
	void Update(Player* player, KamataEngine::Model2* model) override;
};

class PlayerStateDeathDrop : public IPlayerState {
public:
	void Update(Player* player, KamataEngine::Model2* model) override;
};

class PlayerStateDeathDisappear : public IPlayerState {
public:
	void Update(Player* player, KamataEngine::Model2* model) override;
};
