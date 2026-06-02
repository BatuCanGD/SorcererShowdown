#include "code/header/CharacterCreator/AI/Randomized.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"

Character* Randomized::GetTarget(Character* user, Battlefield& bf){
    std::vector<Character*> targets;

    for (const auto& ch : bf.battlefield) {
        if (ch.get() != user)
            targets.push_back(ch.get());
    }
    if (targets.empty()) return nullptr;

    return targets[static_cast<size_t>(Utilities::GetRandomNumber(0, static_cast<int>(targets.size() - 1)))];
}

void Randomized::UseRCT(Sorcerer* user) {
    bool start_caring = !user->HPMoreThanMax(0.75);
    bool critical_hp = !user->HPMoreThanMax(0.20); 

    bool high_ce = user->CEMoreThanMax(0.35);    
    bool scrap_ce = user->CEMoreThanMax(0.05);     

    int roll = Utilities::GetRandomNumber(1, 100);

    if (start_caring && high_ce && roll <= 60) {
        user->BoostRCT();
    }
    else if (critical_hp && scrap_ce && roll <= 90) {
        user->BoostRCT();
    }
    else if (start_caring && scrap_ce && roll <= 40) {
        user->EnableRCT();
    }
    else {
        user->DisableRCT();
    }
}

void Randomized::UseReinforcement(CurseUser* user) {
    if (!user->HPMoreThanMax(0.35)) user->SetCurrentReinforcement(user->GetMaxReinforcement()); 
    else if (!user->HPMoreThanMax(0.50)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.5); 
    else if (!user->HPMoreThanMax(0.75)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.25);
    else if (user->CEMoreThanMax(0.40)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.10); 
    else user->SetCurrentReinforcement(0.0); 
}

void Randomized::UseShikigami(CurseUser* user) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.30)) shiki->Manifest(); 
        else if (shiki->IsActivePhysically() && !user->CEMoreThanMax(0.15)) shiki->Withdraw(); 
    }
}

bool Randomized::TryDomainActions(CurseUser* user, Battlefield&, Character*) {
    if (user->GetCounterDomain() && !user->CounterDomainActive() && !user->DomainActive()) {
        user->ActivateCounterDomain(); 
        if (user->CounterDomainActive()) return true;
    }
    if (!user->HPMoreThanMax(0.40) && user->GetDomain() && !user->DomainActive()) {
        if (Utilities::GetRandomNumber(1, 100) >= 90 && user->GetDomain()->GetDomainUses() >= 5) {
            user->ActivateDomain();
            return true;
        }
        else if (user->GetDomain()->GetDomainUses() < 5) {
            user->ActivateDomain(); 
            return true; 
        }
    }
    return false; 
}

bool Randomized::TryTechniqueActions(CurseUser* user, Battlefield& bf, Character* target) {
    if (user->GetTechnique() && !user->GetTechnique()->BurntOut()) {
        if (!user->HPMoreThanMax(0.50) || user->GetTechnique()->Boosted()) {
            if (user->GetTechnique()->AutoTechniqueUse(user, target, bf)) {
                return true;
            }
        }
    }
    return false; 
}

bool Randomized::TryInventoryActions(Character* user, Character*) {
    const auto& inv = user->GetCursedTools(); 
    if (!inv.empty() && !user->GetTool()) {
        user->CursedToolChoice(static_cast<size_t>(Utilities::GetRandomNumber(1, static_cast<int>(inv.size())))); 
        return true; 
    }
    return false; 
}

std::unique_ptr<CharacterBrain> Randomized::Clone() const {
    return std::make_unique<Randomized>(*this);
}