#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class Purple : public SubTechnique {
public:
    Purple();
    bool UsedMoreThanAmount() const override;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};