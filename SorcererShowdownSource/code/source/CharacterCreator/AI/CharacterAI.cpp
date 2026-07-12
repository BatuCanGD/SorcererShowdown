#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/GameManagement/BattlefieldHeader.h"

void CharacterBrain::ExecuteTurn(Character* user, Battlefield& bf) {
    GetTarget(user, bf);
    if (!t_rex.target) return;

    if (user->IsaCurseUser()) { auto* cu = static_cast<CurseUser*>(user);
        if (cu->IsaSorcerer()) { auto* s = static_cast<Sorcerer*>(cu);
            if (s->HasRCT()){
                UseRCT(s);
            }
        }
        UseReinforcement(cu);
        UseShikigami(cu);
        if (TryDomainActions(cu, bf)) return;
        if (TryTechniqueActions(cu, bf)) return;
    }
    if (TryInventoryActions(user)) return;
    AttackTarget(user);
}

void CharacterBrain::AttackTarget(Character* user){
    if (user->IsaCurseUser()){ auto* crs = static_cast<CurseUser*>(user);
        if (t_rex.needs_amp && !crs->AmpActive()){
            crs->SetAmplification(true);
        }else if(!t_rex.needs_amp && crs->AmpActive()){
            crs->SetAmplification(false);
        }
    }
    user->Attack(t_rex.target);
}