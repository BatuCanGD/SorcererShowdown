#include "Sukuna.h"
#include "BattlefieldHeader.h"
#include "DomainList.h"
#include "Shrine.h"
#include "Limitless.h"
#include "MalevolentShrine.h"
#include "ShikigamiList.h"
#include "WorldCuttingSlash.h"
#include "Utils.h"

#include "PhysicallyGifted.h"




Sukuna::Sukuna() : Sorcerer(1000.0, 20000.0, 200.0) {
    domain = std::make_unique<MalevolentShrine>();
    counter_domain = std::make_unique<HollowWickerBasket>();
    technique = std::make_unique<Shrine>();
    shikigami.push_back(std::make_unique<Mahoraga>());
    shikigami.push_back(std::make_unique<Agito>());
    special = std::make_unique<WorldCuttingSlash>();
    black_flash_chance = 10;
    rct_skill = RCTProficiency::Expert;

    char_name = "Sukuna";
    name_color = "\033[31m";
}

std::unique_ptr<Character> Sukuna::Clone() const {
    return std::make_unique<Sukuna>();
}

void Sukuna::OnCharacterTurn(Character*, Battlefield& bf) {
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
        this->SetCurrentReinforcement(200.0);
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

        score += GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = target.get();
        }
    }

    
    if (GetRandomNumber(1, 20) <= 11) {
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
        if (!shrine->WorldCuttingSlashUnlocked()) {
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

        if (makora->FullyAdapted() && !shrine->WorldCuttingSlashUnlocked()) {
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

    if (shrine->WorldCuttingSlashUnlocked() && this->CEMoreThanMax(0.125) && GetRandomNumber(1, 100) >= 65) {
        if (makora && makora->IsActive()) {
            makora->Withdraw();
        }
        if (!shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (shrine->FullyChanted() && this->CEMoreThanMax(0.125)) {
            shrine->UseTheWorldCuttingSlash(this, strongest);
            return;
        }
    }
    if (!domain_users.empty()) {
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
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
        if (!shrine->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (GetRandomNumber(1, 100) <= 20) {
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

    if (needs_da) this->SetAmplification(true);
    else if (this->DomainAmplificationActive()) this->SetAmplification(false);

    if (!needs_da) {
        if (GetRandomNumber(1, 100) <= 25 && !shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
        if (this->CEMoreThanMax(0.050)) {
            if (strongest->GetCharacterHealth() < strongest->GetCharacterMaxHealth() * 0.25 && GetRandomNumber(1, 100) <= 15) {
                shrine->UseCleave(this, strongest);
                return;
            }
            else if (strongest->CanBeHit()) {
                shrine->UseDismantle(this, strongest);
                return;
            }
        }
    }

    this->Attack(strongest);

    if (needs_da) {
        this->SetAmplification(false);
    }
}



