#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Copy : public Technique {
private:
	std::vector<std::unique_ptr<Technique>> copied_techniques;
	static constexpr int max_copies = 5;
	static constexpr double copy_cost = 500.0;
	size_t active_copy = 0;
public:
	Copy();
	std::unique_ptr<Technique> Clone() const override;

	bool CopyFrom(CurseUser* user, CurseUser* target);
	bool SwitchCopy(size_t index);
	Technique* GetActive() const;
	void Set(Status s) override;

	bool TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	bool TechniqueSetting(CurseUser* user, Battlefield&) override;
	void Chant() override;
	bool IsCopy() const override;
};