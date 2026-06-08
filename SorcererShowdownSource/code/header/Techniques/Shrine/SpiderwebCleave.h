#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;

class SpiderwebCleave : public SubTechnique {
public:
    SpiderwebCleave();
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};