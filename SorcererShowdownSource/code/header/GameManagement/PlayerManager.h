#pragma once
class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;
struct BattleCreator;

class PlayerManager {
protected:
	Character* player = nullptr; CurseUser* crs = nullptr; Sorcerer* src = nullptr;
	bool player_type_found = false;
public:
	void OnPlayerTurn(Character* s, Battlefield& bf);
	void PlayerRCTusage();
	void PlayerDAusage();
	void PlayerDomainUsage();
	void PlayerShikigami();
	void PlayerReinforcement();
	void PlayerVows();
	void GetPlayerTools();
	Character* TargetSelector(Battlefield&);

};