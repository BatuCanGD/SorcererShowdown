#include "code/header/Characters/CurseUsers/Sorcerers/Gojo.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Techniques/Limitless/Blue.h"
#include "code/header/Techniques/Limitless/Red.h"
#include "code/header/Techniques/Limitless/Purple.h"
#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"
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
    base_attack_damage = 100.0;
    rct_skill = RCTProficiency::Absolute;
    name = "Gojo";
    color = "\033[96m";
}
std::unique_ptr<Character> Gojo::Clone() const {
    return std::make_unique<Gojo>();
}

void Gojo::OnCharacterTurn(Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    auto* limitless = static_cast<Limitless*>(this->GetTechnique());
    auto red = limitless->GetRed(); auto blue = limitless->GetBlue(); 
    auto purple = limitless->GetPurple(); auto unlimitedpurple = purple->GetUnlimitedHollowPurple();

    if (!limitless->CheckInfinity() && this->CEMoreThanMax(0.03) && !limitless->BurntOut()) {
        limitless->SetInfinity(true);
    }
    if ((!this->HPMoreThanMax(0.35) && this->CEMoreThanMax(0.25)) || !limitless->CheckInfinity()) 
    {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.75) && this->CEMoreThanMax(0.15))
    {
        this->EnableRCT();
    }
    else 
    {
        this->DisableRCT();
    }
    
    if (this->CEMoreThanMax(0.50) || (unlimitedpurple->CanBeUsed() && limitless->FullyChanted()) || !this->HPMoreThanMax(0.25)) {
        this->SetCurrentReinforcement(200.0);
    }
    else if (this->CEMoreThanMax(0.30)) {
        this->SetCurrentReinforcement(100.0);
    }
    else if (this->CEMoreThanMax(0.20)) {
        this->SetCurrentReinforcement(50.0);
    }
    else {
        this->SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this) continue;

        double hp_ratio = target->GetCharacterHealth() / this->GetCharacterMaxHealth();
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
        this->Taunt(strongest);
    }

    if 
    (blue->UsedMoreThanAmount() && 
    red->UsedMoreThanAmount() && 
    purple->UsedMoreThanAmount() && 
    tntroll >= 70)
    {
        if (!unlimitedpurple->CanBeUsed()) {
            this->GetSpecial()->PerformSpecial(this);
            return;
        }
    }
    
    if (!domain_users.empty()) {
        if (!limitless->BurntOut() && this->GetDomainUses() < 6 && !this->DomainActive()) {
            if (domain_users.size() == 1) {
                this->ActivateDomain(); 
                return;
            }
            else if (Utilities::GetRandomNumber(1, 100) <= 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!this->CounterDomainActive() && !this->DomainActive() && !this->counter_on_cooldown) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
            return;
        }
        if (!limitless->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (Utilities::GetRandomNumber(1, 100) <= 30) {
                this->ActivateDomain();
                return;
            }
        }
    }
    
    if (InfCheck(strongest)) {
        this->SetAmplification(true);

    }
    else {
        this->SetAmplification(false);
    }

    if (strongest && !limitless->BurntOut() && this->CEMoreThanMax(0.03) && !this->DomainAmplificationActive()) {
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

        if ((roll <= 15 && this->CEMoreThanMax(0.35)) || (!purple->UsedMoreThanAmount() && roll >= 70)) {
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
    this->Attack(strongest);
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