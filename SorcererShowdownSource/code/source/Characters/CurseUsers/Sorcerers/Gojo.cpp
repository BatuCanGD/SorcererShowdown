#include "code/header/Characters/CurseUsers/Sorcerers/Gojo.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Limitless/LimitlessTechnique.h"
#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/GameManagement/Utils.h"
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
    auto red = limitless->GetRed(); auto blue = limitless->GetBlue(); 
    auto purple = limitless->GetPurple(); auto unlimitedpurple = purple->GetUnlimitedHollowPurple();

    if (!limitless->CheckInfinity() && CEMoreThanMax(0.01) && !limitless->BurntOut()) {
        limitless->SetInfinity(true);
    }
    if ((!HPMoreThanMax(0.35) && CEMoreThanMax(0.03)) || !limitless->CheckInfinity()) 
    {
        BoostRCT();
    }
    else if (!HPMoreThanMax(0.75) && CEMoreThanMax(0.15))
    {
        EnableRCT();
    }
    else 
    {
        DisableRCT();
    }
    
    if (CEMoreThanMax(0.50) || (unlimitedpurple->CanBeUsed() && limitless->FullyChanted()) || !HPMoreThanMax(0.25)) {
        SetCurrentReinforcement(200.0);
    }
    else if (CEMoreThanMax(0.30)) {
        SetCurrentReinforcement(100.0);
    }
    else if (CEMoreThanMax(0.20)) {
        SetCurrentReinforcement(50.0);
    }
    else {
        SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this) continue;

        double hp_ratio = target->GetCharacterHealth() / GetCharacterMaxHealth();
        double score = hp_ratio;

        if (target->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(target.get());
            if (curse_user->DomainActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }
            if (auto* tech = curse_user->GetTechnique()) {
                if (tech->IsShrine()) {
                    score += 1.0;
                }
                if (tech->IsLimitless()) {
                    score += (tech->IsInfinityActive() ? 0.30 : 0.15);
                }
            }
        }
        else if (target->IsPhysicallyGifted()) {
            score += 0.25;
        }
        score += Utilities::GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = target.get();
        }

    }
    int tntroll = Utilities::GetRandomNumber(1, 100);

    if (tntroll <= 45) {
        Taunt(strongest);
    }

    if 
    (blue->UsedMoreThanAmount() && 
    red->UsedMoreThanAmount() && 
    purple->UsedMoreThanAmount() && 
    tntroll >= 70)
    {
        if (!unlimitedpurple->CanBeUsed()) {
            GetSpecial()->PerformSpecial(this);
            return;
        }
    }
    
    if (!domain_users.empty()) {
        if (!limitless->BurntOut() && GetDomain()->GetDomainUses() < 6 && !DomainActive()) {
            if (domain_users.size() == 1) {
                ActivateDomain(); 
                return;
            }
            else if (Utilities::GetRandomNumber(1, 100) <= 1) {
                ActivateDomain();
                return;
            }
        }
        else if (!CounterDomainActive() && !DomainActive() && !counter_on_cooldown) {
            ActivateCounterDomain();
            return;
        }
    }
    else {
        if (CounterDomainActive()) {
            DeactivateCounterDomain();
            return;
        }
        if (!limitless->BurntOut() && GetDomain()->GetDomainUses() < 5 && !DomainActive()) {
            if (Utilities::GetRandomNumber(1, 100) <= 30) {
                ActivateDomain();
                return;
            }
        }
    }
    
    if (InfCheck(strongest)) {
        SetAmplification(true);

    }
    else {
        SetAmplification(false);
    }

    if (strongest && !limitless->BurntOut() && CEMoreThanMax(0.03) && !DomainAmplificationActive()) {
        int roll = Utilities::GetRandomNumber(1, 100); int croll = Utilities::GetRandomNumber(1, 10);

        if ((croll <= 4 && !limitless->FullyChanted()) || 
            (unlimitedpurple->CanBeUsed() && !limitless->FullyChanted() && !unlimitedpurple->UsedMoreThanAmount())) 
        {
            limitless->Chant();
            return;
        }
        if (limitless->FullyChanted() && unlimitedpurple->CanBeUsed() && !unlimitedpurple->UsedMoreThanAmount()) {
            unlimitedpurple->UseTechnique(this, strongest, bf, limitless->GetChantLevel());
            return;
        }

        if ((roll <= 15 && CEMoreThanMax(0.35)) || (!purple->UsedMoreThanAmount() && roll >= 70)) {
            purple->UseTechnique(this, strongest, bf, limitless->GetChantLevel());
        }
        else if (roll <= 60 && !blue->UsedMoreThanAmount()) {
            blue->UseTechnique(this, strongest, bf, limitless->GetChantLevel());;
        }
        else {
            red->UseTechnique(this, strongest, bf, limitless->GetChantLevel());
        }
        return;
    }
    Attack(strongest);
}

bool Gojo::InfCheck(Character* strongest) {
    bool needs_amplification = false;
    if (strongest->IsaCurseUser()) {
        auto curse_user = static_cast<CurseUser*>(strongest);
        if (auto* tech = curse_user->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) {
                needs_amplification = true;
            }
        }
    }
    return needs_amplification;
}