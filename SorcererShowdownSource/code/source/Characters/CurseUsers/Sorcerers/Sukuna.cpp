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
    can_use_rct = true;
    black_flash_chance = 10;
    attack_damage = 90.0;
    max_reinforcement = 250.0;
    reinforcement_cost_mult = 1.85;
    rct_skill = RCTProficiency::Absolute;

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
    auto* WCS = GetSpecial();
    if (!HPMoreThanMax(0.25) && CEMoreThanMax(0.15)){
        BoostRCT();
    }
    else if (!HPMoreThanMax(0.75)){
        EnableRCT();
    }
    else{
        DisableRCT();
    }

    if (CEMoreThanMax(0.75) || !HPMoreThanMax(0.15) || WCS->CheckSpecial(this)) {
        SetCurrentReinforcement(GetMaxReinforcement());
    }
    else if (CEMoreThanMax(0.50)) {
        SetCurrentReinforcement(100.0);
    }
    else if (CEMoreThanMax(0.25)) {
        SetCurrentReinforcement(50.0);
    }
    else {
        SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& target : bf.battlefield) {
        if (target.get() == this || target->GetCharacterHealth() <= 0.0) continue;

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
                    score += (tech->HasInvulnerabilityBarrier() ? 0.30 : 0.45);
                }
            }
        }
        else if (target->IsPhysicallyGifted()) {
            score += 0.25;
        }

        score += Utilities::GetRandom(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = target.get();
        }
    }
    if (!strongest) return;
    
    if (Utilities::GetRandom(1, 20) <= 11) {
        Taunt(strongest);
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
    Shrine* shrine = static_cast<Shrine*> (GetTechnique());
    if (makora) {
        if (!WCS->CheckSpecial(this)) {
            if (!makora->IsActivePhysically() && CEMoreThanMax(0.40)) {
                makora->Manifest();
            }
            else if (!makora->IsActive() && CEMoreThanMax(0.025)) {
                makora->PartiallyManifest();
            }
            else if (!CEMoreThanMax(0.35)) {
                makora->Withdraw();
            }
        }
        if (makora->FullyAdapted() && makora->IsActive()) {
            makora->Withdraw();
            return;
        }
    }

    if (agito && !HPMoreThanMax(0.35)) {
        if (!CEMoreThanMax(0.30) && agito->IsActive()) {
            agito->Withdraw();
        }
        else if (!agito->IsActive() && HPMoreThanMax(0.50) && CEMoreThanMax(0.30)) {
            agito->Manifest();
        }
    }

    if (Utilities::GetRandom(1, 100) >= 65) {
        if (!shrine->FullyChanted()) {
            shrine->Chant();
            return;
        }
    }
    if (!domain_users.empty()) {
        if (!shrine->BurntOut() && GetDomain()->GetDomainUses() < domain_limit && !DomainActive()) {
            if (domain_users.size() == 1) {
                ActivateDomain();
                return;
            }
            else if (Utilities::GetRandom(1, 100) <= 1) {
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
        if (!shrine->BurntOut() && GetDomain()->GetDomainUses() < domain_limit && !DomainActive()) {
            if (Utilities::GetRandom(1, 100) <= 20) {
                ActivateDomain();
                return;
            }
        }
    }

    bool needs_da = false;
    if (strongest->IsaCurseUser()) {
        auto* cu = static_cast<CurseUser*>(strongest);
        if (auto* ct = cu->GetTechnique()) {
            if (ct->HasInvulnerabilityBarrier()) needs_da = true;
        }
    }
    if (WCS->CheckSpecial(this)) needs_da = false; // dont need it if the special is ready to use

    if (needs_da) {
        SetAmplification(true);
    }
    else if (DomainAmplificationActive()) {
        SetAmplification(false);
    } 

    if (!needs_da && !shrine->BurntOut()) {
        if ((Utilities::GetRandom(1, 100) <= 25 && !shrine->FullyChanted()) || (!shrine->FullyChanted() && WCS->CheckSpecial(this))) {
            shrine->Chant();
            return;
        }
        if (shrine->FullyChanted() && WCS->CheckSpecial(this) && CEMoreThanMax(0.15)) {
            WCS->UseSpecial(this, strongest, bf);
            return;
        }

        if (CEMoreThanMax(0.050)) {
            if (strongest->GetCharacterHealth() < strongest->GetCharacterMaxHealth() * 0.25 && Utilities::GetRandom(1, 100) <= 15) {
                shrine->GetCleave()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
                return;
            }else if (!HPMoreThanMax(0.25) && strongest->HPMoreThanMax(0.50) && shrine->GetChantLevel() >= Technique::ChantLevel::One){
                shrine->GetCleave()->GetSpiderwebCleave()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
                return;
            }
            else {
                shrine->GetDismantle()->UseTechnique(this, strongest, bf, shrine->GetChantLevel());
                return;
            }
        }
    }
    Attack(strongest);
}



