#include "code/header/CharacterCreator/AI/Randomized.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/GameManagement/VList.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"

void Randomized::GetTarget(Character* user, Battlefield& bf){
    t_rex.target = VList::TargetChooser(user, bf);
    if (t_rex.target->IsaCurseUser()){
        auto* t = static_cast<CurseUser*>(t_rex.target);
        if (t->GetTechnique() && t->GetTechnique()->HasInvulnerabilityBarrier()){
            t_rex.needs_amp = true;
        }else if(t_rex.needs_amp){
            t_rex.needs_amp = false;
        }
    }
}

void Randomized::UseRCT(Sorcerer* user) {
    bool start_caring = !user->HPMoreThanMax(0.75);
    bool critical_hp = !user->HPMoreThanMax(0.20); 

    bool high_ce = user->CEMoreThanMax(0.35);    
    bool scrap_ce = user->CEMoreThanMax(0.05);     

    int roll = Utilities::GetRandom(1, 100);

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

bool Randomized::TryDomainActions(CurseUser* user, Battlefield& bf) {
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
        if (can_clash_safely || Utilities::GetRandom<int>(1,100) >= 50) {
            bool under_limit = domain->GetDomainUses() < user->GetDomainLimit();

            if (domain_count == 0 && (under_limit || Utilities::GetRandom<int>(1, 100) >= 50)) {
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

bool Randomized::TryTechniqueActions(CurseUser* user, Battlefield& bf) {
    auto* tech = user->GetTechnique();
    if (!tech) return false;
    if (user->GetTechnique() && !user->GetTechnique()->BurntOut() && !t_rex.needs_amp) {
        if (Utilities::GetRandom<int>(1, 100) >= 50) {
            if (user->GetTechnique()->AutoTechniqueUse(user, t_rex.target, bf)) {
                return true;
            }
        }
    }
    return false; 
}

bool Randomized::TryInventoryActions(Character* user) {
    const auto& inv = user->GetCursedTools(); 
    if (!inv.empty() && !user->GetTool()) {
        user->CursedToolChoice(Utilities::GetRandom<size_t>(1, static_cast<int>(inv.size()))); 
        return true; 
    }
    return false; 
}

std::unique_ptr<CharacterBrain> Randomized::Clone() const {
    return std::make_unique<Randomized>(*this);
}