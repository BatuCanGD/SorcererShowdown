#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class IdleTransfiguration : public Technique {
protected:
	int transfigured_human_count = 3;
	const double transfiguration_damage = 100.0;
public:
	IdleTransfiguration();

	void UseTransfiguration(CurseUser* user, Character* target);
	void SummonTransfiguredHumans(Battlefield&);
	void GetTransfiguredSummonAmount(Battlefield&);

	int GetTFcount() const;

	void Chant() override;
	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	void AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;
	std::unique_ptr<Technique> Clone() const override;
};