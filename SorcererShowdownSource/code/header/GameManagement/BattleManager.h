#pragma once

class CurseUser;
struct Battlefield;
struct BattleCreator;

class BattleManager {
private:
    Battlefield& bf; BattleCreator& bc;
	void loadSetup(bool);
	void DoSurehit(CurseUser*);
public:
    BattleManager(Battlefield& battlefield, BattleCreator& battlecreator) 
	: bf(battlefield), bc(battlecreator) {}

	void SpawnNewFighters();
	void ClearCharacters();
	void ManageEndOfTurn();
	void DomainCheckAndPerform();

	bool GameEndCheck(bool spectator_mode);
	bool SetupBattlefield();
	bool PlayerSearch(bool);
	bool IsBattleOver(bool,bool,bool);
	bool GameEndChoice();
	std::pair<bool, bool> SkipTurnFullyCheck();
};