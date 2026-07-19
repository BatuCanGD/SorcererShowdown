#pragma once
#include "code/header/Techniques/Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Limitless : public Technique {
protected:
	bool Infinity = true;
	static constexpr double red_dmg = 200.0;
	static constexpr double blu_dmg = 175.0;
	static constexpr double prl_dmg = 300.0;

	int use_amount{};
public:
	Limitless();
	std::unique_ptr<Technique> Clone() const override;

	bool CanUseUnlimitedHollow() const;

	void UseBlue(CurseUser*, Character*);
	void UseRed(CurseUser*, Character*);
	void UsePurple(CurseUser*, Character*);

	bool TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	bool TechniqueSetting(CurseUser*, Battlefield&) override;
	void Chant() override;

	bool IsLimitless() const override;
	bool HasInvulnerabilityBarrier() const override;
	void SetInvulnerabilityBarrier(bool) override;
};
