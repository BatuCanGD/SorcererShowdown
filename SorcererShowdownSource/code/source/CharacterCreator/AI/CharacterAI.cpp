#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/GameManagement/Utils.h"




void CharacterBrain::ExecuteTurn(Character* user, Battlefield& bf) {
    Character* target = GetTarget(user, bf);
    if (!target) return;

    if (user->IsaCurseUser()) {
        auto* cu = static_cast<CurseUser*>(user);
        if (cu->IsaSorcerer()) {
            UseRCT(static_cast<Sorcerer*>(cu));
        }
        UseReinforcement(cu); 
        UseShikigami(cu);
        if (TryDomainActions(cu, bf, target)) return;
        if (TryTechniqueActions(cu, bf, target)) return;
    }
    if (TryInventoryActions(user, target)) return;

    user->Attack(target);
}