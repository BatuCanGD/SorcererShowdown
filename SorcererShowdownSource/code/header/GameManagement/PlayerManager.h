#pragma once

class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;

class PlayerManager {
private:
	Character* TargetSelector(Battlefield&);
	void GetPlayerTools(Character*);
	void PlayerDAusage(CurseUser*);
	void PlayerDomainUsage(CurseUser*);
	void PlayerShikigami(CurseUser*);
	void PlayerReinforcement(CurseUser*);
	void PlayerVows(CurseUser*);
	void PlayerRCTusage(Sorcerer*);
public:
	void OnPlayerTurn(Character* player, Battlefield& bf);
};