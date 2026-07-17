#include "code/header/Characters/CurseUsers/Sorcerers/Gojo.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/VList.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"



Gojo::Gojo() : Sorcerer(800.0, 12500.0, 150.0) {
    domain = std::make_unique<InfiniteVoid>();
    counter_domain = std::make_unique<SimpleDomain>();
    technique = std::make_unique<Limitless>();
    special = std::make_unique<UnlimitedPurple>();
    six_eyes = true;
    black_flash_chance = 15;
    attack_damage = 115.0;
    reinforcement_cost_mult = 1.25;
    rct_skill = RCTProficiency::Absolute;
    ce_efficiency = CEfficiency::Absolute;
    can_use_rct = true;
    name = "Gojo";
    color = "\033[96m";
}
std::unique_ptr<Character> Gojo::Clone() const {
    return std::make_unique<Gojo>();
}

void Gojo::OnCharacterTurn(Battlefield& bf) {
    if (IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", GetNameWithID());
        return;
    }
    auto* limitless = static_cast<Limitless*>(GetTechnique());
    auto* unlimited_purple = GetSpecial();
    bool can_use_barrier = !limitless->HasInvulnerabilityBarrier() && CEMoreThanMax(0.005) && !limitless->BurntOut();
    bool can_use_uhp = limitless->CanUseUnlimitedHollow() && unlimited_purple->CheckSpecial(this) && !limitless->BurntOut();

    if (can_use_barrier) {
        limitless->SetInvulnerabilityBarrier(true);
    }
    if ((CEMoreThanMax(0.60) && !HPMoreThanMax(0.65)) || can_use_uhp){
        SetCurrentReinforcement(200.0);
    }else if (CEMoreThanMax(0.05) && HPMoreThanMax(0.65)){
        SetCurrentReinforcement(100.0 + Utilities::GetRandom<double>(-15.0, 75.0));
    }else {
        SetCurrentReinforcement(Utilities::GetRandom<double>(10.0, 25.0));
    }

    if (!HPMoreThanMax(0.20)){
        BoostRCT();
    }else if(!HPMoreThanMax(0.65)){
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
                    score += (tech->HasInvulnerabilityBarrier() ? 0.10 : 0.45);
                }
            }
        }
        else if (target->IsPhysicallyGifted()) {
            score += 0.25;
        }
        score += Utilities::GetRandom(-5, 5) * 0.01;

        if (score > best_score || !strongest) {
            best_score = score;
            strongest = target.get();
        }

    }

    if (Utilities::GetRandom(1, 100) <= 45) Taunt(strongest);

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
        if (d_size == 0 && Utilities::GetRandom<int>(1, 100) >= 60){
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

    bool amplification_needed = VList::DoINeedAmplification(strongest) && !unlimited_purple->CheckSpecial(this);
    SetAmplification(amplification_needed);
    bool can_use_technique = !limitless->BurntOut() && CEMoreThanMax(0.005);

    if (can_use_technique && !amplification_needed){
        if (can_use_uhp){
            if (!limitless->FullyChanted()){
                limitless->Chant();
            }else {
                unlimited_purple->UseSpecial(this, strongest, bf);
            }
            return;
        }
        int roll = Utilities::GetRandom<int>(1, 10);
        if (roll >= 7){
            limitless->UsePurple(this, strongest);
        }else if (roll >= 4){
            limitless->UseRed(this, strongest);
        }else{
            limitless->UseBlue(this, strongest);
        }
        return;
    }
    Attack(strongest);
}
