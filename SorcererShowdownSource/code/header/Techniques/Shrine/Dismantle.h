#pragma once
#include "code/header/Techniques/SubTechniques.h"
#include "code/header/Techniques/Shrine/WorldCuttingSlash.h"

class CurseUser;
class Character;

class Dismantle : public SubTechnique {
protected:
    std::unique_ptr<WorldCuttingShrine> world_cutting_slash = nullptr;
public:
    Dismantle();
    WorldCuttingShrine* GetWorldCuttingSlash() const;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};