#include "CharacterCreator/AI/Randomized.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "Characters/Shikigami/Shikigami.h"
#include "GameManagement/VList.h"
#include "GameManagement/Utils.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Techniques/Techniques.h"
#include "Domains/Domain.h"
#include "Specials/Specials.h"

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
    if(!user->HPMoreThanMax(Utilities::GetRandom(0.00, 1.00))){
        user->SetRCTAmount(Utilities::GetRandom(0.0, 500.0));
    }else{
        user->SetRCTAmount(0.0);
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
        else if (shiki->IsActive() && !user->CEMoreThanMax(0.15)) shiki->Withdraw(); 
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
    if (Technique* tech = user->GetTechnique()){
        if (!tech->BurntOut() && !t_rex.needs_amp && !user->AmpActive() && Utilities::GetRandom<int>(1, 100) >= 50) {
            return user->GetTechnique()->AutoTechniqueUse(user, t_rex.target, bf);
        }
    }
    if (Specials* sp = user->GetSpecial()){
        if (Utilities::GetRandom(1, 100) <= 50) {
            return sp->UseSpecial(user, t_rex.target, bf);
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