#include "code/header/Characters/CurseUsers/Sorcerers/Gojo.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"



Gojo::Gojo() : Sorcerer(800.0, 4000.0, 150.0) {
    domain = std::make_unique<InfiniteVoid>();
    counter_domain = std::make_unique<SimpleDomain>();
    technique = std::make_unique<Limitless>();
    special = std::make_unique<UnlimitedPurple>();
    six_eyes = true;
    black_flash_chance = 15;
    rct_skill = RCTProficiency::Absolute;
    char_name = "Gojo";
    name_color = "\033[96m";
}
std::unique_ptr<Character> Gojo::Clone() const {
    return std::make_unique<Gojo>();
}

void Gojo::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    auto* limitless = static_cast<Limitless*>(this->GetTechnique());
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
    
    if (this->CEMoreThanMax(0.50) || (limitless->UnlimitedHollowAllowed() && limitless->FullyChanted()) || !this->HPMoreThanMax(0.25)) {
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
    bool shrine_found = false;

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
        score += GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = target.get();
        }

    }
    int tntroll = GetRandomNumber(1, 100);

    if (tntroll <= 45) {
        this->Taunt(strongest);
    }

    if ((limitless->UPBlueCheck() && limitless->UPRedCheck() && limitless->UPPurpleCheck() && tntroll >= 70)) {
        if (!limitless->UnlimitedHollowAllowed()) {
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
            else if (GetRandomNumber(1, 100) <= 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!(this->CounterDomainActive() && this->DomainActive()) && !this->counter_on_cooldown) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
        }
        if (!limitless->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (GetRandomNumber(1, 100) <= 30) {
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

    if (strongest && !limitless->BurntOut() && this->CEMoreThanMax(0.05) && !this->DomainAmplificationActive()) {
        int roll = GetRandomNumber(1, 100);
        int croll = GetRandomNumber(1, 10);

        if ((croll <= 4 && !limitless->FullyChanted()) || 
            (limitless->UnlimitedHollowAllowed() && !limitless->FullyChanted() && !limitless->UnlimitedHollowUsed())) 
        {
            limitless->Chant();
            return;
        }
        if (limitless->FullyChanted() && limitless->UnlimitedHollowAllowed() && !limitless->UnlimitedHollowUsed()) {
            limitless->UseUnlimitedHollowPurple(this, bf);
            return;
        }

        if (roll <= 15 && this->CEMoreThanMax(0.35)) {
            limitless->UsePurple(this, strongest);
        }
        else if (roll <= 60) {
            limitless->UseBlue(this, strongest);
        }
        else {
            limitless->UseRed(this, strongest);
        }
        return;
    }
    this->Attack(strongest);
    if (this->DomainAmplificationActive()) this->SetAmplification(false);
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