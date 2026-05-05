#include "CharacterAI.h"
#include "Character.h"
#include "Sorcerer.h"
#include "CurseUser.h"
#include "PhysicallyGifted.h"
#include "Techniques.h"
#include "Limitless.h"
#include "Shikigami.h"
#include "Domain.h"
#include "Specials.h"
#include "Specials.h"
#include "CursedTool.h"
#include "BattlefieldHeader.h"
#include "Utils.h"



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