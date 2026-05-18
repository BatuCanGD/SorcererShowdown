#include "code/header/CharacterCreator/AI/Reactive.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"

Character* Reactive::GetTarget(Character* user, Battlefield& bf){
    Character* target = nullptr;
    double best_score = -1.0;

    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user) continue;

        double score = 1.0 - (ch->GetCharacterHealth() / ch->GetCharacterMaxHealth());

        if (ch->IsaCurseUser()) {
            auto cu = static_cast<CurseUser*>(ch.get());

            if (cu->DomainActive()) score -= 0.5;

            if (auto* tech = cu->GetTechnique()) {
                if (tech->IsShrine()) score -= 0.5;
                if (tech->IsLimitless()) score -= 0.2;
            }
        }
        else if (ch->IsPhysicallyGifted()) {
            score += 0.8;
        }

        score += Utilities::GetRandomNumber(-5, 5) * 0.025;

        if (score > best_score) {
            best_score = score;
            target = ch.get();
        }
    }

    return target;
}

void Reactive::UseRCT(Sorcerer* user) {
    bool critical_hp = !user->HPMoreThanMax(0.25); 
    bool low_hp = !user->HPMoreThanMax(0.50);

    bool massive_ce = user->CEMoreThanMax(0.50);  
    bool healthy_ce = user->CEMoreThanMax(0.20);  
    bool scrap_ce = user->CEMoreThanMax(0.05);  

    if (low_hp && massive_ce) {
        user->BoostRCT();
    }
    else if (critical_hp && scrap_ce) {
        user->BoostRCT();
    }
    else if (low_hp && healthy_ce) {
        user->EnableRCT();
    }
    else {
        user->DisableRCT();
    }
}

void Reactive::UseReinforcement(CurseUser* user) {
    if (!user->HPMoreThanMax(0.35)) user->SetCurrentReinforcement(user->GetMaxReinforcement()); 
    else if (!user->HPMoreThanMax(0.50)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.5); 
    else if (!user->HPMoreThanMax(0.75)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.25);
    else if (user->CEMoreThanMax(0.40)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.10);
    else if (user->CEMoreThanMax(0.40)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.10); 
    else user->SetCurrentReinforcement(0.0); 
}

void Reactive::UseShikigami(CurseUser* user) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.30)) shiki->Manifest(); 
        else if (shiki->IsActivePhysically() && !user->CEMoreThanMax(0.15)) shiki->Withdraw(); 
    }
}

bool Reactive::TryDomainActions(CurseUser* user, Battlefield& bf, Character*) {
    std::vector<CurseUser*> domain_users; 
    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user) continue; 
        if (!ch->IsaCurseUser()) continue; 
        auto* crs = static_cast<CurseUser*>(ch.get()); 
        if (crs->GetDomain() && crs->DomainActive()) domain_users.push_back(crs); 
    }

    if (!domain_users.empty()) {
        if (user->GetDomain() && !user->DomainActive() && user->GetDomainUses() < 5 && !user->IsStrained()) {
            if (!user->GetTechnique() || !user->GetTechnique()->BurntOut())
            user->ActivateDomain();
            return true;
        }
        if (user->GetCounterDomain() && !user->CounterDomainActive() && !user->DomainActive()) {
            user->ActivateCounterDomain(); 
            if (user->CounterDomainActive()) return true; 
        }
    }
    else {
        if (user->CounterDomainActive() && Utilities::GetRandomNumber(1, 10) >= 6) {
            user->DeactivateCounterDomain(); 
            return true;
        }
        if (user->GetDomain() && !user->DomainActive() && !user->IsStrained() && user->GetDomainUses() < 5 && Utilities::GetRandomNumber(1, 100) >= 60) {
            if (!user->GetTechnique() || !user->GetTechnique()->BurntOut()) {
                user->ActivateDomain();
                return true;
            }
        }
    }
    return false; 
}

bool Reactive::TryTechniqueActions(CurseUser* user, Battlefield& bf, Character* target) {
    bool target_infinity = false; 
    if (target->IsaCurseUser()) {
        auto tr = static_cast<CurseUser*>(target); 
            if (auto* tech = tr->GetTechnique()) {
                if (tech->IsLimitless() && tech->IsInfinityActive()) target_infinity = true; 
            }
    }

    if (target_infinity) {
        user->SetAmplification(true);
    }
    else if (user->DomainAmplificationActive()) {
        user->SetAmplification(false);
    }

    if (user->GetTechnique() && !user->GetTechnique()->BurntOut() && !user->DomainAmplificationActive()) {
        if (!user->HPMoreThanMax(0.50) || user->GetTechnique()->Boosted()) {
            if (user->GetTechnique()->AutoTechniqueUse(user, target, bf)) {
                return true;
            }
        }
    }
    if (user->GetSpecial() && Utilities::GetRandomNumber(1, 100) <= 20) {
        user->GetSpecial()->PerformSpecial(user);
    }
    return false; 
}

bool Reactive::TryInventoryActions(Character* user, Character* target) {
    const auto& inv = user->GetCursedTools(); 
    auto* tool = user->GetTool(); 

    if (inv.empty() && !tool) {
        return false; 
    }

    bool target_infinity = false; 
    if (target->IsaCurseUser()) {
        auto tr = static_cast<CurseUser*>(target); 
        if (auto* tech = tr->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) target_infinity = true; 
        }
    }

    if (target_infinity) {
        if (tool && tool->IsAntiTechniqueWeapon()) return false; 

        for (size_t i = 0; i < inv.size(); ++i) {
            if (inv[i]->IsAntiTechniqueWeapon()) {
                user->CursedToolChoice(i + 1); 
                return true; 
            }
        }
    }
    else if (!inv.empty() && !tool) {
        if (Utilities::GetRandomNumber(1, 100) <= 50) {
            user->CursedToolChoice(static_cast<size_t>(Utilities::GetRandomNumber(1, static_cast<int>(inv.size())))); 
            return true; 
        }
    }
    else if (tool && !inv.empty()) {
        if (Utilities::GetRandomNumber(1, 100) <= 25) {
            user->CursedToolChoice(static_cast<size_t>(Utilities::GetRandomNumber(1, static_cast<int>(inv.size())))); 
            return true; 
        }
    }
    return false; 
}

std::unique_ptr<CharacterBrain> Reactive::Clone() const {
    return std::make_unique<Reactive>(*this);
}