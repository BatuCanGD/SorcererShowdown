#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/GameManagement/BattlefieldHeader.h"

void CharacterBrain::ExecuteTurn(Character* user, Battlefield& bf) {
    Character* target = GetTarget(user, bf);
    if (!target) return;

    if (user->IsaCurseUser()) {
        auto* cu = static_cast<CurseUser*>(user);
        if (cu->IsaSorcerer()) {
            auto* s = static_cast<Sorcerer*>(cu);
            if (s->HasRCT()){
                UseRCT(s);
            }
        }
        UseReinforcement(cu); 
        UseShikigami(cu);
        if (TryDomainActions(cu, bf, target)) return;
        if (TryTechniqueActions(cu, bf, target)) return;
    }
    if (TryInventoryActions(user, target)) return;

    user->Attack(target);
}