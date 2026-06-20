#pragma once

class Character;
class CurseUser;
class Sorcerer;

struct Battlefield;

struct PlayerType{
	friend class PlayerManager;
	Character* player = nullptr;
	CurseUser* crs = nullptr;
	Sorcerer* src = nullptr;

	PlayerType(Character* pl) : player(pl) {}
	PlayerType() = default;
private:
	void FindPlayerType();
};


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
	PlayerManager(Character* user) : pt(user) {
		pt.FindPlayerType();
	}
	void OnPlayerTurn(Battlefield& bf);
};