#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;
class SpiderwebCleave;

class Cleave : public SubTechnique {
protected:
    std::unique_ptr<SpiderwebCleave> spiderweb_cleave = nullptr;
public:
    Cleave();
    virtual ~Cleave();
    SpiderwebCleave* GetSpiderwebCleave() const;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};