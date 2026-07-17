#include "code/header/Characters/CurseUsers/Sorcerers/Sukuna.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/Characters/Shikigami/Agito.h"
#include "code/header/Domains/MalevolentShrine.h"
#include "code/header/Domains/HollowWickerBasket.h"
#include "code/header/Techniques/Shrine.h"
#include "code/header/Specials/WorldCuttingSlash.h"
#include "code/header/GameManagement/VList.h"
#include "code/header/GameManagement/Utils.h"


Sukuna::Sukuna() : Sorcerer(1000.0, 20000.0, 300.0) {
    domain = std::make_unique<MalevolentShrine>();
    counter_domain = std::make_unique<HollowWickerBasket>();
    technique = std::make_unique<Shrine>();
    shikigami.push_back(std::make_unique<Mahoraga>());
    shikigami.push_back(std::make_unique<Agito>());
    special = std::make_unique<WorldCuttingSlash>();
    can_use_rct = true;
    black_flash_chance = 10;
    attack_damage = 90.0;
    max_reinforcement = 250.0;
    reinforcement_cost_mult = 1.85;
    rct_skill = RCTProficiency::Absolute;
    ce_efficiency = CEfficiency::Expert;

    name = "Sukuna";
    color = "\033[31m";
}

std::unique_ptr<Character> Sukuna::Clone() const {
    return std::make_unique<Sukuna>();
}

void Sukuna::OnCharacterTurn(Battlefield& bf) {
    if (IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", GetNameWithID());
        return;
    }
    auto* shrine = static_cast<Shrine*>(GetTechnique());
    auto* wcs = GetSpecial();
    bool can_use_wcs = wcs->CheckSpecial(this) && shrine->FullyChanted() && !shrine->BurntOut();
    Mahoraga* maho = static_cast<Mahoraga*>(shikigami[0].get());
    Agito* agito = static_cast<Agito*>(shikigami[1].get());

    if ((CEMoreThanMax(0.60) && !HPMoreThanMax(0.65)) || can_use_wcs){
        SetCurrentReinforcement(250.0);
    }else if (CEMoreThanMax(0.05) && HPMoreThanMax(0.65)){
        SetCurrentReinforcement(100.0 + Utilities::GetRandom<double>(-35.0, 105.0));
    }else {
        SetCurrentReinforcement(Utilities::GetRandom<double>(10.0, 25.0));
    }

    if (!HPMoreThanMax(0.15)){
        BoostRCT();
    }else if(!HPMoreThanMax(0.45)){
        EnableRCT();
    }else{
        DisableRCT();
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this || target->GetCharacterHealth() <= 0.0) continue;

        double hp_ratio = target->GetCharacterHealth() / target->GetCharacterMaxHealth();
        double score = hp_ratio;

        if (target->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(target.get());

            if (curse_user->GetDomain() && curse_user->GetDomain()->IsActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }
            if (auto* tech = curse_user->GetTechnique()) {
                if (tech->IsShrine()) {
                    score += 1.0;
                }
                if (tech->IsLimitless()) {
                    score += (tech->HasInvulnerabilityBarrier() ? 0.30 : 0.45);
                }
            }
        }
        else if (target->IsPhysicallyGifted()) {
            score += 0.25;
        }

        score += Utilities::GetRandom(-5.0, 5.0) * 0.01;

        if (score > best_score || !strongest) {
            best_score = score;
            strongest = target.get();
        }
    }
    
    if (Utilities::GetRandom(1, 20) <= 11) Taunt(strongest);

    if (maho){
        if (!wcs->CheckSpecial(this)) {
            if (!maho->IsActive()) {
                maho->Manifest();
            }
            else if (!CEMoreThanMax(0.35)) {
                maho->Withdraw();
            }
        }
        if (maho->FullyAdapted() && maho->IsActive()) {
            maho->Withdraw();
        }
    }
    if (agito){
        if (CEMoreThanMax(0.35) && !HPMoreThanMax(0.50)) {
            if (!agito->IsActive()) {
                agito->Manifest();
            }
        }else{
            agito->Withdraw();
        }
    }

    const size_t d_size = domain_users.size();
    bool usable_domain = !domain->OnCooldown() && 
                         !domain->IsActive() && 
                          domain->GetDomainUses() < 5;
    bool usable_counter = !counter_domain->OnCooldown() && 
                          !counter_domain->IsActive();

    if (usable_domain) {
        if (d_size == 1){
            domain->SetDomainActivation(this, true);
            return;
        }
        if (d_size == 0 && Utilities::GetRandom<int>(1, 100) >= 20){
            domain->SetDomainActivation(this, true);
            return;
        }
    }
    if (usable_counter && !domain->IsActive()){
        if (d_size == 1){
            counter_domain->SetDomainActivation(this, true);
            return;
        }else if(counter_domain->IsActive()){
            counter_domain->SetDomainActivation(this, false);
            return;
        }
    }

    bool amplification_needed = VList::DoINeedAmplification(strongest) && !wcs->CheckSpecial(this);
    SetAmplification(amplification_needed);
    bool can_use_technique = !shrine->BurntOut() && CEMoreThanMax(0.005);

    if (can_use_technique && !amplification_needed){
        if (can_use_wcs){
            if (!shrine->FullyChanted()){
                shrine->Chant();
            }else {
                wcs->UseSpecial(this, strongest, bf);
            }
            return;
        }
        int roll = Utilities::GetRandom(1, 25);
        if (roll >= 18){
            if (roll >= 21 && static_cast<int>(shrine->GetChantLevel()) >= 1){
                shrine->UseSpiderweb(this, bf);
            }else{
               shrine->UseCleave(this, strongest); 
            }
        }else{
            shrine->UseDismantle(this, strongest);
        }
        return;
    }

    Attack(strongest);
}


