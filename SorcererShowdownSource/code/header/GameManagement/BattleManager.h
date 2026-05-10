#pragma once
class Character;
class CurseUser;
struct Battlefield;
struct BattleCreator;

class BattleManager {
public:
	bool GameEndCheck(Battlefield&, bool spectator_mode);
	bool SetupBattlefield(Battlefield&, BattleCreator&);
	static void loadSetup(Battlefield&, BattleCreator&, bool);
	bool SkipTurnFullyCheck();
	void SpawnNewFighters(Battlefield&);
	bool ManageEndOfTurn(Battlefield&, bool spectator_mode);
	void DomainCheckAndPerform(Battlefield&);
	static void DoSurehit(CurseUser*,Battlefield&);
	bool IsBattleOver(bool,bool,bool, Battlefield&);
};