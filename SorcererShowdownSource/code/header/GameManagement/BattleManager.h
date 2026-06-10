#pragma once
class Character;
class CurseUser;
struct Battlefield;
struct BattleCreator;

class BattleManager {
public:
	bool GameEndCheck(const Battlefield&, bool spectator_mode);
	bool SetupBattlefield(Battlefield&, BattleCreator&);
	static void loadSetup(Battlefield&, BattleCreator&, bool);
	std::pair<bool, bool> SkipTurnFullyCheck();
	void SpawnNewFighters(Battlefield&);
	void ClearCharacters(Battlefield&);
	bool PlayerSearch(Battlefield&, bool);
	void ManageEndOfTurn(Battlefield&);
	void DomainCheckAndPerform(Battlefield&);
	static void DoSurehit(CurseUser*,Battlefield&);
	bool IsBattleOver(bool,bool,bool, Battlefield&);
	bool GameEndChoice();
};