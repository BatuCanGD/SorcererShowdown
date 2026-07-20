#pragma once
#include "CharacterCreator/AI/CharacterAI.h"

struct Reactive : public CharacterBrain {
public:
    std::unique_ptr<CharacterBrain> Clone() const override;
protected:
    void GetTarget(Character* user, Battlefield& bf) override;
    void UseRCT(Sorcerer* user) override;
    void UseReinforcement(CurseUser* user) override;
    void UseShikigami(CurseUser* user) override;
    bool TryDomainActions(CurseUser* user, Battlefield& bf) override;
    bool TryTechniqueActions(CurseUser* user, Battlefield& bf) override;
    bool TryInventoryActions(Character* user) override;
};