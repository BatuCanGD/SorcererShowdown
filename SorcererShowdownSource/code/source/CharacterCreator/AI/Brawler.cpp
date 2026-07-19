#include "code/header/CharacterCreator/AI/Brawler.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Shikigami/Shikigami.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Domains/Domain.h"
#include "code/header/Specials/Specials.h"

void Brawler::GetTarget(Character* user, Battlefield& bf){
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

void Brawler::UseRCT(Sorcerer* user) {
    if(!user->HPMoreThanMax(0.10)){
        user->SetRCTAmount(300.0 + Utilities::GetRandom(-200.0, 200.0));
    }else if (!user->HPMoreThanMax(0.40)){
        user->SetRCTAmount(50.0 + Utilities::GetRandom(-25.0, 75.0));
    }else{
        user->SetRCTAmount(0.0);
    }
}

void Brawler::UseReinforcement(CurseUser* user) {
    if (user->CEMoreThanMax(0.50)) user->SetCurrentReinforcement(user->GetMaxReinforcement()); 
    else if (user->CEMoreThanMax(0.30)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.5); 
    else if (user->CEMoreThanMax(0.20)) user->SetCurrentReinforcement(user->GetMaxReinforcement() * 0.25); 
    else user->SetCurrentReinforcement(0.0); 
}

bool Brawler::TryDomainActions(CurseUser* user, Battlefield& bf) {
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

bool Brawler::TryTechniqueActions(CurseUser* user, Battlefield& bf) {
    auto* tech = user->GetTechnique();
    if (!tech) return false;
    
    if (user->GetTechnique() && !user->GetTechnique()->BurntOut() && !t_rex.needs_amp) {
        if (user->CEMoreThanMax(0.20) && Utilities::GetRandom(1, 100) >= 90) {
            return user->GetTechnique()->AutoTechniqueUse(user, t_rex.target, bf);
        }
    }
    if (Specials* sp = user->GetSpecial()){
        if (sp->CheckSpecial(user) && Utilities::GetRandom(1, 100) <= 20) {
            return sp->UseSpecial(user, t_rex.target, bf);
        }
    }
    return false;
}

void Brawler::UseShikigami(CurseUser* user) {
    for (const auto& shiki : user->GetShikigami()) {
        if (!shiki->IsActive() && user->CEMoreThanMax(0.50)) {
            shiki->Manifest(); 
        }
        else if (shiki->IsActive() && !user->CEMoreThanMax(0.35)) {
            shiki->Withdraw(); 
        }
    }
}

bool Brawler::TryInventoryActions(Character* user) {
    const auto& inv = user->GetCursedTools(); 
    auto* tool = user->GetTool();

    if (inv.empty() && !tool) return false;
    else if (Utilities::GetRandom<int>(1, 100) > 25) return false;

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
        if (Utilities::GetRandom(1, 100) >= 100) {
            user->CursedToolChoice(Utilities::GetRandom<size_t>(1, static_cast<int>(inv.size()))); 
            return true;
        }
    }
    else if (tool && !inv.empty()) {
        if (Utilities::GetRandom(1, 100) <= 1) {
            user->CursedToolChoice(Utilities::GetRandom<size_t>(1, static_cast<int>(inv.size()))); 
            return true; 
        }
    }else if (tool && inv.empty()){
        if (Utilities::GetRandom(1, 50) < 15){
            user->CursedToolChoice(0);
            return true;
        }
    }
    return false; 
}

std::unique_ptr<CharacterBrain> Brawler::Clone() const {
    return std::make_unique<Brawler>(*this);
}