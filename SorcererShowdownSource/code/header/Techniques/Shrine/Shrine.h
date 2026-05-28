#pragma once
#include "code/header/Techniques/Techniques.h"
#include "code/header/Techniques/Shrine/Cleave.h"
#include "code/header/Techniques/Shrine/Dismantle.h"

class CurseUser;
class Character;
struct Battlefield;

class Shrine : public Technique { 
protected:
	std::unique_ptr<Dismantle> dismantle = nullptr;
    std::unique_ptr<Cleave> cleave = nullptr;
public:
	Shrine();
	Shrine(const Shrine&);
	std::unique_ptr<Technique> Clone() const override;

	Dismantle* GetDismantle() const;
	Cleave* GetCleave() const;

	void TechniqueMenu(CurseUser* user, Character* target, Battlefield&) override;
	bool AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) override;
	void TechniqueSetting(CurseUser*, Battlefield&) override;
	void Chant() override;
	
	bool IsShrine() const override;
};
