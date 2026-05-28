#pragma once
#include "code/header/Techniques/SubTechniques.h"

class CurseUser;
class Character;
class UnlimitedHollowPurple;

class Purple : public SubTechnique {
protected:
    std::unique_ptr<UnlimitedHollowPurple> unlimited_hollow_purple = nullptr;
public:
    Purple();
    virtual ~Purple();
    bool UsedMoreThanAmount() const override;
	void UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel&) override;
    UnlimitedHollowPurple* GetUnlimitedHollowPurple() const;
};