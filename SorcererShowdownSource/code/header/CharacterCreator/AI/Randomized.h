#pragma once
#include "CharacterAI.h"

struct Randomized: public CharacterBrain {
    std::unique_ptr<CharacterBrain> Clone() const override;
protected:
    Character* GetTarget(Character* user, Battlefield& bf) override;
    void UseRCT(Sorcerer* user) override;
    void UseReinforcement(CurseUser* user) override;
    void UseShikigami(CurseUser* user) override;
    bool TryDomainActions(CurseUser* user, Battlefield& bf, Character* target) override;
    bool TryTechniqueActions(CurseUser* user, Battlefield& bf, Character* target) override;
    bool TryInventoryActions(Character* user, Character* target) override;
};