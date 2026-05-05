#pragma once
#include "std.h"

class Character;
struct Battlefield;

class PlayerManager {
public:
	void OnPlayerTurn(Character& s, Battlefield& bf);
	void PlayerRCTusage(Character& s);
	void PlayerDAusage(Character& s);
	void PlayerDomainUsage(Character& s);
	void PlayerShikigami(Character& s);
	void PlayerReinforcement(Character& s);
	void GetPlayerTools(Character& s);
	Character* TargetSelector(Battlefield&, Character* player);

};