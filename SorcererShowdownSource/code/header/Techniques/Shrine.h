#pragma once
#include "code/header/Techniques/Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Shrine : public Technique { 
protected:
	static constexpr double dsm_dmg = 135.0;
	static constexpr double clv_dmg = 350.0;
	static constexpr double web_dmg = 75.0;
public:
	Shrine();
	std::unique_ptr<Technique> Clone() const override;

	void UseDismantle(CurseUser*, Character*);
	void UseCleave(CurseUser*, Character*);
	void UseSpiderweb(CurseUser*, Battlefield&);

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;
	void Chant() override;
	
	bool IsShrine() const override;
};
