#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class WorldCuttingShrine : public SubTechnique { // name was same as the special, had to change it
protected:
    bool allowed = false;
public:
    WorldCuttingShrine();
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
    void SetAllowance(bool);
    bool CanBeUsed() const override;
};