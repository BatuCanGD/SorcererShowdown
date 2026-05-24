#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;
struct Battlefield;

class UnlimitedHollowPurple : public SubTechnique {
protected:
    bool allowed = false;
public:
    UnlimitedHollowPurple();
    bool UsedMoreThanAmount() const override;
    void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
    void SetAllowance(bool);
    bool CanBeUsed();
};