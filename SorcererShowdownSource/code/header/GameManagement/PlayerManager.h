#pragma once

class Character;
class CurseUser;
class Sorcerer;
struct Battlefield;

class PlayerManager {
private:
	enum class Action{
		Attack,
		Technique,
		TechSettings,
		Special,
		Domain,
		RCT,
		Shikigami,
		BindingVows,
		Taunt,
		Reinforcement,
		CursedTools
	};
	std::vector<Action> DealWithActions(Character*, CurseUser*, Sorcerer*);
	bool DealWithCursedTools(Character*);
	bool DealWithFighting(Character*, Battlefield&);
	bool DealWithTaunting(Character*, Battlefield&);
	bool DealWithBindingVows(CurseUser*);
	bool DealWithReinforcement(CurseUser*);
	bool DealWithSpecial(CurseUser*, Battlefield&);
	bool DealWithTechnique(CurseUser*, Battlefield&);
	bool DealWithTechSettings(CurseUser*, Battlefield&);
	bool DealWithDomain(CurseUser*);
	bool DealWithShikigami(CurseUser*);
	bool DealWithRCT(Sorcerer*);
public:
	void OnPlayerTurn(Character* player, Battlefield& bf);
};