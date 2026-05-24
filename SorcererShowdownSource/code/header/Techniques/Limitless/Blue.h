#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class Blue : public SubTechnique {
public:
    Blue();
    bool UsedMoreThanAmount() const override;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};