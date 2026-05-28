#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;
class WorldCuttingShrine;

class Dismantle : public SubTechnique {
protected:
    std::unique_ptr<WorldCuttingShrine> world_cutting_slash = nullptr;
public:
    Dismantle();
    virtual ~Dismantle();
    WorldCuttingShrine* GetWorldCuttingSlash() const;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
};