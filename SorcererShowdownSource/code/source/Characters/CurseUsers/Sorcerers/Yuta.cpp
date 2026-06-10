#include "code/header/Characters/CurseUsers/Sorcerers/Yuta.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Copy.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Characters/Shikigami/Rika.h"
#include "code/header/Domains/AuthenticMutualLove.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/CursedTools/Katana.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"

Yuta::Yuta() : Sorcerer(800.0, 15000.0, 50.0) {
    technique = std::make_unique<Copy>();
    cursed_tool = std::make_unique<Katana>();
    domain = std::make_unique<AuthenticMutualLove>();
    counter_domain = std::make_unique<SimpleDomain>();
    shikigami.push_back(std::make_unique<Rika>());
    can_use_rct = true;
    black_flash_chance = 10;
    max_reinforcement = 500.0;
    attack_damage = 70.0;
    rct_skill = RCTProficiency::Adept;

    name = "Yuta Okkotsu";
    color = "\033[38;5;183m";
}

std::unique_ptr<Character> Yuta::Clone() const {
    return std::make_unique<Yuta>();
}

void Yuta::OnCharacterTurn(Battlefield& bf) {
    if (IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", GetNameWithID());
        return;
    }
    Shikigami* rika = ChooseShikigami(0);

    if (!HPMoreThanMax(0.50) || !CEMoreThanMax(0.20)) {
        if (!(rika->GetActiveTime() >= 5) && !rika->IsActivePhysically()) {
            std::println("Come, Rika.");
            rika->Manifest();
        }
    }

    if (!HPMoreThanMax(0.40) || rika->IsActivePhysically()) {
        BoostRCT();
    }
    else if (!HPMoreThanMax(0.55)) {
        EnableRCT();
    }
    else {
        DisableRCT();
    }

    if (rika->IsActivePhysically()){
        SetCurrentReinforcement(500.0);
    }
    else if (CEMoreThanMax(0.60) || !HPMoreThanMax(0.20)) {
        SetCurrentReinforcement(200.0);
    }
    else if (CEMoreThanMax(0.20)) {
        SetCurrentReinforcement(100.0);
    }
    else {
        SetCurrentReinforcement(50.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& s : bf.battlefield) {
        if (s.get() == this || s->GetCharacterHealth() <= 0) continue;
        double score = s->GetCharacterHealth() / GetCharacterMaxHealth();

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

    if (!strongest){
        while(true){
            std::println("SOMETHING FAILED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
    }

    int tntroll = Utilities::GetRandomNumber(1, 20);
    if (tntroll <= 4) {
        Taunt(strongest);
    }

    if (!domain_users.empty()) {
        if (!GetTechnique()->BurntOut() && GetDomain()->GetDomainUses() < 5 && !DomainActive()) {
            if (domain_users.size() == 1) {
                ActivateDomain();
                return;
            }
        }
        else if (!DomainActive() && !CounterDomainActive() && !counter_on_cooldown) {
            ActivateCounterDomain();
            return;
        }
    }
    else {
        if (CounterDomainActive()) {
            DeactivateCounterDomain();
            return;
        }
        if (!GetTechnique()->BurntOut() && GetDomain()->GetDomainUses() < 5 && !DomainActive()) {
            if (Utilities::GetRandomNumber(1, 100) <= 25) {
                ActivateDomain();
                return;
            }
        }
    }
    
    if (InfCheck(strongest)) {
        SetAmplification(true);
    }
    else if (DomainAmplificationActive()) {
        SetAmplification(false);
    }

    if (strongest && !GetTechnique()->BurntOut() && !DomainAmplificationActive()) {
        if (GetTechnique()->AutoTechniqueUse(this, strongest, bf)) {
            return;
        }
    }
    Attack(strongest);
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
