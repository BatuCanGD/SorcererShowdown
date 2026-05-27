#include "code/header/Characters/CurseUsers/Sorcerers/Sukuna.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Domains/DomainList.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Techniques/Shrine/ShrineTechnique.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Domains/MalevolentShrine.h"
#include "code/header/Characters/Shikigami/ShikigamiList.h"
#include "code/header/Specials/WorldCuttingSlash.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"


Sukuna::Sukuna() : Sorcerer(1000.0, 20000.0, 300.0) {
    domain = std::make_unique<MalevolentShrine>();
    counter_domain = std::make_unique<HollowWickerBasket>();
    technique = std::make_unique<Shrine>();
    shikigami.push_back(std::make_unique<Mahoraga>());
    shikigami.push_back(std::make_unique<Agito>());
    special = std::make_unique<WorldCuttingSlash>();
    black_flash_chance = 10;
    base_attack_damage = 90.0;
    max_reinforcement = 250.0;
    reinforcement_cost_mult = 2.0;
    rct_skill = RCTProficiency::Absolute;

    name = "Sukuna";
    color = "\033[31m";
}

std::unique_ptr<Character> Sukuna::Clone() const {
    return std::make_unique<Sukuna>();
}

void Sukuna::OnCharacterTurn(Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    if (!this->HPMoreThanMax(0.25) && this->CEMoreThanMax(0.15))
    {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.75))
    {
        this->EnableRCT();
    }
    else
    {
        this->DisableRCT();
    }

    if (this->CEMoreThanMax(0.75) || !this->HPMoreThanMax(0.15)) {
        this->SetCurrentReinforcement(this->GetMaxReinforcement());
    }
    else if (this->CEMoreThanMax(0.50)) {
        this->SetCurrentReinforcement(100.0);
    }
    else if (this->CEMoreThanMax(0.25)) {
        this->SetCurrentReinforcement(50.0);
    }
    else {
        this->SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this || target->GetCharacterHealth() <= 0.0) continue;

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
    if (!strongest) return;
    
    if (Utilities::GetRandomNumber(1, 20) <= 11) {
        this->Taunt(strongest);
    }

    Mahoraga* makora = nullptr;
    Agito* agito = nullptr;

    for (const auto& s : shikigami) {
        if (s->IsMahoraga()) {
            makora = static_cast<Mahoraga*>(s.get());
        }
        else if (s->IsAgito()) {
            agito = static_cast<Agito*>(s.get());
        }
    }
    Shrine* shrine = static_cast<Shrine*> (this->GetTechnique());
    if (makora) {
        if (!shrine->GetDismantle()->GetWorldCuttingSlash()->CanBeUsed()) {
            if (!makora->IsActivePhysically() && this->CEMoreThanMax(0.40)) {
                makora->Manifest();
            }
            else if (!makora->IsActive() && this->CEMoreThanMax(0.025)) {
                makora->PartiallyManifest();
            }
            else if (!this->CEMoreThanMax(0.35)) {
                makora->Withdraw();
            }
        }

        if (makora->FullyAdapted() && !shrine->GetDismantle()->GetWorldCuttingSlash()->CanBeUsed()) {
            this->GetSpecial()->PerformSpecial(this);
            makora->Withdraw();
            return;
        }
    }

    if (agito && !this->HPMoreThanMax(0.35)) {
        if (!this->CEMoreThanMax(0.30) && agito->IsActive()) {
            agito->Withdraw();
        }
        else if (!agito->IsActive() && this->HPMoreThanMax(0.50) && this->CEMoreThanMax(0.30)) {
            agito->Manifest();
        }
    }

    if (shrine->GetDismantle()->GetWorldCuttingSlash()->CanBeUsed() && this->CEMoreThanMax(0.125) && Utilities::GetRandomNumber(1, 100) >= 65) {
        if (makora && makora->IsActive()) {
            makora->Withdraw();
        }
        if (!shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (shrine->FullyChanted() && this->CEMoreThanMax(0.125)) {
            shrine->GetDismantle()->GetWorldCuttingSlash()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
            return;
        }
    }
    if (!domain_users.empty()) {
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (domain_users.size() == 1) {
                this->ActivateDomain();
                return;
            }
            else if (Utilities::GetRandomNumber(1, 100) <= 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!this->CounterDomainActive() && !this->DomainActive() && !counter_on_cooldown) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
            return;
        }
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (Utilities::GetRandomNumber(1, 100) <= 20) {
                this->ActivateDomain();
                return;
            }
        }
    }

    bool needs_da = false;
    if (strongest->IsaCurseUser()) {
        auto* cu = static_cast<CurseUser*>(strongest);
        if (auto* ct = cu->GetTechnique()) {
            if (ct->IsLimitless() && ct->IsInfinityActive()) needs_da = true;
        }
    }

    if (needs_da) {
        this->SetAmplification(true);
    }
    else if (this->DomainAmplificationActive()) {
        this->SetAmplification(false);
    } 

    if (!needs_da && !shrine->BurntOut()) {
        if (Utilities::GetRandomNumber(1, 100) <= 25 && !shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (this->CEMoreThanMax(0.050)) {
            if (strongest->GetCharacterHealth() < strongest->GetCharacterMaxHealth() * 0.25 && Utilities::GetRandomNumber(1, 100) <= 15) {
                shrine->GetCleave()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
                return;
            }
            else if (strongest->CanBeHit()) {
                shrine->GetDismantle()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
                return;
            }
        }
    }
    this->Attack(strongest);
}



