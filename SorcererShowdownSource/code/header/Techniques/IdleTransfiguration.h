#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class IdleTransfiguration : public Technique {
protected:
	int transfigured_human_count = 3;
	static constexpr double transfiguration_damage = 100.0;
public:
	IdleTransfiguration();

	void UseTransfiguration(CurseUser* user, Character* target);
	void SummonTransfiguredHumans(Battlefield&);
	void GetTransfiguredSummonAmount(Battlefield&);

	int GetTFcount() const;

	void Chant() override;
	bool TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	bool TechniqueSetting(CurseUser*, Battlefield&) override;
	std::unique_ptr<Technique> Clone() const override;
};