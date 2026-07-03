#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class Dismantle : public SubTechnique {
public:
    Dismantle();
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};