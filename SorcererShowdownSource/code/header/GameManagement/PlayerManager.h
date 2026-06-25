#pragma once
#include "code/header/GameManagement/PlayerFinder.h"

class Character;
struct Battlefield;

class PlayerManager {
private:
	PlayerType pt;
	Character* TargetSelector(Battlefield&);
	void PlayerRCTusage();
	void PlayerDAusage();
	void PlayerDomainUsage();
	void PlayerShikigami();
	void PlayerReinforcement();
	void PlayerVows();
	void GetPlayerTools();
public:
	PlayerManager() = default;
	PlayerManager(Character* user) : pt(user) { pt.FindPlayerType(); }
	void OnPlayerTurn(Battlefield& bf);
};