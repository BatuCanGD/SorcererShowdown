#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class Red : public SubTechnique {
public:
    Red();
    bool UsedMoreThanAmount() const override;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};