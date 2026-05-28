#pragma once
#include "code/header/Techniques/SubTechniques.h"
#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"

class CurseUser;
class Character;

class Purple : public SubTechnique {
protected:
    std::unique_ptr<UnlimitedHollowPurple> unlimited_hollow_purple = nullptr;
public:
    Purple();
    bool UsedMoreThanAmount() const override;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
    UnlimitedHollowPurple* GetUnlimitedHollowPurple() const;
};