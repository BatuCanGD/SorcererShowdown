#pragma once
#include "Techniques.h"

class CurseUser;
class Character;
struct Battlefield;

class Copy : public Technique {
private:
	std::vector<std::unique_ptr<Technique>> copied_techniques;
	static constexpr int max_copies = 5;
	size_t active_copy = 0;
public:
	Copy();
	std::unique_ptr<Technique> Clone() const override;

	void CopyFrom(CurseUser* user, CurseUser* target);
	void SwitchCopy(size_t index);
	Technique* GetActive() const;
	void Set(Status s) override;

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	void TechniqueSetting(CurseUser* user, Battlefield&) override;
	void Chant() override;
	bool IsCopy() const override;
};