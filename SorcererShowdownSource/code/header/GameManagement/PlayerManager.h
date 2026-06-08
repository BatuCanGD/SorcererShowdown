#pragma once
class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;
struct BattleCreator;

class PlayerManager {
protected:
	CurseUser* crs = nullptr; Sorcerer* src = nullptr;
	bool player_type_found = false;
public:
	void OnPlayerTurn(Character* s, Battlefield& bf);
	void PlayerRCTusage(Sorcerer*);
	void PlayerDAusage(CurseUser*);
	void PlayerDomainUsage(CurseUser*);
	void PlayerShikigami(CurseUser*);
	void PlayerReinforcement(CurseUser*);
	void PlayerVows(CurseUser*);
	void GetPlayerTools(Character* s);
	Character* TargetSelector(Battlefield&, Character* player);

};