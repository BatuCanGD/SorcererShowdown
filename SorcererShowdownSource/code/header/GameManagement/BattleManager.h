#pragma once

class CurseUser;
struct Battlefield;
struct BattleCreator;

class BattleManager {
private:
    Battlefield& bf; BattleCreator& bc;
	unsigned int turncount{1};
	void loadSetup(bool);
	void DoSurehit(CurseUser*);
public:
    BattleManager(Battlefield& battlefield, BattleCreator& battlecreator) 
	: bf(battlefield), bc(battlecreator) {}

	void SpawnNewFighters();
	void ClearCharacters();
	void ManageEndOfTurn(bool);
	void DomainCheckAndPerform();

	bool GameEndCheck(bool spectator_mode);
	bool SetupBattlefield();
	bool PlayerSearch(bool);
	bool IsBattleOver(bool,bool,bool);
	std::tuple<bool, bool, bool> SkipCheck();
};