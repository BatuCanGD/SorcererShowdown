#include "code/header/CharacterCreator/AI/Aggressive.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"

void Aggressive::GetTarget(Character* user, Battlefield& bf){
    Character* target = nullptr;
    double best_score = -1.0;

    for (const auto& ch : bf.battlefield) {
        if (ch.get() == user) continue;

        double score = ch->GetCharacterHealth() / ch->GetCharacterMaxHealth();

        if (ch->IsaCurseUser()) {
            auto cu = static_cast<CurseUser*>(ch.get());

            if (cu->GetDomain()->IsActive()) score += 1.0;

            if (auto* tech = cu->GetTechnique()) {
                if (tech->IsShrine()) score += 0.55;
                if (tech->IsLimitless()) score += 0.60;
            }
        }
        else if (ch->IsPhysicallyGifted()) {
            score += 0.5;
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

void Aggressive::UseRCT(Sorcerer* user) {
    bool critical_hp = !user->HPMoreThanMax(0.20); 
    bool bruised_hp = !user->HPMoreThanMax(0.50);

    bool plenty_ce = user->CEMoreThanMax(0.40); 
    bool enough_ce = user->CEMoreThanMax(0.15); 

    if (bruised_hp && plenty_ce) {
        user->BoostRCT();
    }
    else if (critical_hp && enough_ce) {
        user->BoostRCT();
    }
    else if (bruised_hp && enough_ce) {
        user->EnableRCT();
    }
    else {
        user->DisableRCT();
    }
}

void Aggressive::UseReinforcement(CurseUser* user) {
    if (user->CEMoreThanMax(0.50)) user->SetCurrentReinforcement(user->GetMaxReinforcement()); 
    else if (user->CEMoreThanMax(0.30)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.5); 
    else if (user->CEMoreThanMax(0.20)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.25); 
    else user->SetCurrentReinforcement(0.0); 
}

bool Aggressive::TryDomainActions(CurseUser* user, Battlefield& bf) {
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

    if (counter && !counter->IsActive() && !counter->OnCooldown()) {
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

bool Aggressive::TryTechniqueActions(CurseUser* user, Battlefield& bf) {
    auto* tech = user->GetTechnique();
    if (!tech) return false;

    if (tech && !tech->BurntOut() && !user->AmpActive() && user->CEMoreThanMax(0.20)) {
        if (tech->AutoTechniqueUse(user, t_rex.target, bf)) return true;
    }
    if (Specials* sp = user->GetSpecial()){
        if (sp->CheckSpecial(user) && Utilities::GetRandom(1, 100) <= 20) {
            sp->UseSpecial(user, t_rex.target, bf);
        }
    }
    return false;
}

void Aggressive::UseShikigami(CurseUser* user) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.30)) {
            shiki->Manifest(); 
        }
        else if (shiki->IsActivePhysically() && !user->CEMoreThanMax(0.15)) {
            shiki->Withdraw(); 
        }
    }
}

bool Aggressive::TryInventoryActions(Character* user) {
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

std::unique_ptr<CharacterBrain> Aggressive::Clone() const {
    return std::make_unique<Aggressive>(*this);
}