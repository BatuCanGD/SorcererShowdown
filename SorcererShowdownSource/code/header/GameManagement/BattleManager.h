#pragma once
#include "std.h"

class Character;
struct Battlefield;

class BattleManager {
public:
	bool GameEndCheck(Battlefield&, bool spectator_mode);
	bool SetupBattlefield(Battlefield&);
	void loadSetup(Battlefield&, bool);
	bool SkipTurnFullyCheck();
	void SpawnNewFighters(Battlefield&);
	bool ManageEndOfTurn(Battlefield&, bool spectator_mode);
	void DomainCheckAndPerform(Battlefield&);
	bool IsBattleOver(bool,bool,bool, Battlefield&);
};