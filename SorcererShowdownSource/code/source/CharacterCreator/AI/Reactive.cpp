#include "CharacterCreator/AI/Reactive.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "Characters/Shikigami/Shikigami.h"
#include "CursedTools/CursedTool.h"
#include "GameManagement/Utils.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Techniques/Techniques.h"
#include "Domains/Domain.h"
#include "Specials/Specials.h"

void Reactive::GetTarget(Character* user, Battlefield& bf){
    Character* target = nullptr;
    double best_score = -1.0;

    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user) continue;

        double score = 1.0 - (ch->GetCharacterHealth() / ch->GetCharacterMaxHealth());

        if (ch->IsaCurseUser()) {
            auto cu = static_cast<CurseUser*>(ch.get());

            if (auto* domain = cu->GetDomain(); domain && domain->IsActive()) {
                score += 1.0;
            }

            if (auto* tech = cu->GetTechnique()) {
                if (tech->IsShrine()) score -= 0.5;
                if (tech->IsLimitless()) score -= 0.2;
            }
        }
        else if (ch->IsPhysicallyGifted()) {
            score += 0.8;
        }

        score += Utilities::GetRandom(-5, 5) * 0.025;

        if (score > best_score) {
            best_score = score;
            target = ch.get();
        }
    }

    if (target->IsaCurseUser()){
        auto* tr = static_cast<CurseUser*>(target);
        if (auto* tech = tr->GetTechnique()){
            if (tech->HasInvulnerabilityBarrier()){
                t_rex.needs_amp  = true;
            }else if(t_rex.needs_amp){
                t_rex.needs_amp = false;
            }
        }
    }
    t_rex.target = target;
}

void Reactive::UseRCT(Sorcerer* user) {
    if(!user->HPMoreThanMax(0.25)){
        user->SetRCTAmount(250.0 + Utilities::GetRandom(-150.0, 50.0));
    }else if (!user->HPMoreThanMax(0.85)){
        user->SetRCTAmount(100.0 + Utilities::GetRandom(-75.0, 45.0));
    }else{
        user->SetRCTAmount(0.0);
    }
}

void Reactive::UseReinforcement(CurseUser* user) {
    if (!user->HPMoreThanMax(0.35)) user->SetCurrentReinforcement(user->GetMaxReinforcement()); 
    else if (!user->HPMoreThanMax(0.50)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.5); 
    else if (!user->HPMoreThanMax(0.75)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.25);
    else if (user->CEMoreThanMax(0.40)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.10);
    else user->SetCurrentReinforcement(0.0); 
}

void Reactive::UseShikigami(CurseUser* user) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.30)) shiki->Manifest(); 
        else if (shiki->IsActive() && !user->CEMoreThanMax(0.15)) shiki->Withdraw(); 
    }
}

bool Reactive::TryDomainActions(CurseUser* user, Battlefield& bf) {
    auto* domain = user->GetDomain();
    auto* counter = user->GetCounter();
    if (!domain && !counter) return false;
    
    std::vector<CurseUser*> domain_users;
    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user || !ch->IsaCurseUser()) continue;  

        auto* crs = static_cast<CurseUser*>(ch.get()); 
        if (crs->GetDomain() && crs->GetDomain()->IsActive()) {
            domain_users.push_back(crs); 
        }
    }

    size_t domain_count = domain_users.size();
    bool can_clash_safely = domain_count <= 1;

    if (domain && !domain->IsActive() && !domain->OnCooldown()) {
        if (can_clash_safely) {
            bool under_limit = domain->GetDomainUses() < user->GetDomainLimit();

            if (domain_count == 0 && (under_limit || Utilities::GetRandom<int>(1, 100) >= 80)) {
                domain->SetDomainActivation(user, true);
                return domain->IsActive();
            }
            
            if (domain_count == 1) {
                domain->SetDomainActivation(user, true);
                return domain->IsActive();
            }
        }
    }
    
    if (counter && !counter->IsActive() && !counter->OnCooldown() && !domain->IsActive()) {
        if (domain_count == 1) {
            counter->SetDomainActivation(user, true);
            return counter->IsActive();
        }
    }else if (counter && counter->IsActive() && domain_count == 0){
        counter->SetDomainActivation(user, false);
        return !counter->IsActive();
    }
    return false;
}

bool Reactive::TryTechniqueActions(CurseUser* user, Battlefield& bf) {
    if (Technique* tech = user->GetTechnique()){
        if (!tech->BurntOut() && !t_rex.needs_amp && user->AmpActive() && (!user->HPMoreThanMax(0.50) || user->GetTechnique()->Boosted())) {
            return user->GetTechnique()->AutoTechniqueUse(user, t_rex.target, bf);
        }
    }
    if (Specials* sp = user->GetSpecial()){
        if (Utilities::GetRandom(1, 100) <= 75) {
            return sp->UseSpecial(user, t_rex.target, bf);
        }
    }
    return false; 
}

bool Reactive::TryInventoryActions(Character* user) {
    const auto& inv = user->GetCursedTools(); 
    auto* tool = user->GetTool(); 

    if (inv.empty() && !tool) {
        return false; 
    }

    if (t_rex.needs_amp) {
        if (tool && tool->IsAntiTechniqueWeapon()) return false; 

        for (size_t i = 0; i < inv.size(); ++i) {
            if (inv[i]->IsAntiTechniqueWeapon()) {
                user->CursedToolChoice(i + 1); 
                return true; 
            }
        }
    }
    else if (!inv.empty() && !tool) {
        if (Utilities::GetRandom(1, 100) <= 50) {
            user->CursedToolChoice(Utilities::GetRandom<size_t>(1, static_cast<int>(inv.size()))); 
            return true; 
        }
    }
    else if (tool && !inv.empty()) {
        if (Utilities::GetRandom(1, 100) <= 25) {
            user->CursedToolChoice(Utilities::GetRandom<size_t>(1, static_cast<int>(inv.size()))); 
            return true; 
        }
    }
    return false; 
}

std::unique_ptr<CharacterBrain> Reactive::Clone() const {
    return std::make_unique<Reactive>(*this);
}