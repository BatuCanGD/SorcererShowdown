#include "code/header/Characters/CurseUsers/Sorcerers/Yuta.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Copy.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Characters/Shikigami/Rika.h"
#include "code/header/Domains/AuthenticMutualLove.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/CursedTools/Katana.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"

Yuta::Yuta() : Sorcerer(800.0, 15000.0, 10.0) {
    technique = std::make_unique<Copy>();
    cursed_tool = std::make_unique<Katana>();
    domain = std::make_unique<AuthenticMutualLove>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    black_flash_chance = 10;
    base_attack_damage = 70.0;
    rct_skill = RCTProficiency::Adept;

    char_name = "Yuta Okkotsu";
    name_color = "\033[38;5;183m";
}

std::unique_ptr<Character> Yuta::Clone() const {
    return std::make_unique<Yuta>();
}

void Yuta::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetNameWithID());
        return;
    }
    Shikigami* rika = this->ChooseShikigami(0);

    if (!this->HPMoreThanMax(0.50) || !this->CEMoreThanMax(0.20)) {
        if (!(rika->GetActiveTime() >= 5) && !rika->IsActivePhysically()) {
            std::println("Come, Rika.");
            rika->Manifest();
        }
    }

    if (!this->HPMoreThanMax(0.40) || rika->IsActivePhysically()) {
        this->BoostRCT();
    }
    else if (!this->HPMoreThanMax(0.55)) {
        this->EnableRCT();
    }
    else {
        this->DisableRCT();
    }

    if (this->CEMoreThanMax(0.60) || rika->IsActive() || !this->HPMoreThanMax(0.20)) {
        this->SetCurrentReinforcement(200.0);
    }
    else if (this->CEMoreThanMax(0.20)) {
        this->SetCurrentReinforcement(100.0);
    }
    else {
        this->SetCurrentReinforcement(50.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& s : bf.battlefield) {
        if (s.get() == this || s->GetCharacterHealth() <= 0) continue;
        double score = s->GetCharacterHealth() / this->GetCharacterMaxHealth();

        if (s->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(s.get());
            if (curse_user->DomainActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }

            if (auto* tech = curse_user->GetTechnique()) {
                if (tech->IsShrine()) score += 0.80;
                if (tech->IsLimitless()) score += 0.15;
            }
        }
        else if (s->IsPhysicallyGifted()) {
            score += 0.25;
        }

        score += Utilities::GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = s.get();
        }
    }

    int tntroll = Utilities::GetRandomNumber(1, 20);
    if (tntroll <= 4) {
        this->Taunt(strongest);
    }

    if (!domain_users.empty()) {
        if (!this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (domain_users.size() == 1) {
                this->ActivateDomain();
                return;
            }
        }
        else if (!this->DomainActive() && !this->CounterDomainActive() && !this->counter_on_cooldown) {
            this->ActivateCounterDomain();
            return;
        }
    }
    else {
        if (this->CounterDomainActive()) {
            this->DeactivateCounterDomain();
            return;
        }
        if (!this->GetTechnique()->BurntOut() && this->GetDomainUses() < 5 && !this->DomainActive()) {
            if (Utilities::GetRandomNumber(1, 100) <= 25) {
                this->ActivateDomain();
                return;
            }
        }
    }
    
    if (InfCheck(strongest)) {
        this->SetAmplification(true);
    }
    else if (this->DomainAmplificationActive()) {
        this->SetAmplification(false);
    }

    if (strongest && !this->GetTechnique()->BurntOut() && !this->DomainAmplificationActive()) {
        if (this->GetTechnique()->AutoTechniqueUse(this, strongest, bf)) {
            return;
        }
    }
    this->Attack(strongest);
}

bool Yuta::InfCheck(Character* strongest) {
    bool needs_amplification = false;
    if (!strongest) return false;
    if (strongest->IsaCurseUser()) {
        auto crs = static_cast<CurseUser*>(strongest);
        if (auto* tech = crs->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) {
                needs_amplification = true;
            }
        }
    }
    return needs_amplification;
}
