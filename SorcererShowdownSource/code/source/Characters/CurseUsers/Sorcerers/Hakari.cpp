#include "code/header/Characters/CurseUsers/Sorcerers/Hakari.h"
#include "code/header/Techniques/PrivatePureLoveTrain.h"
#include "code/header/Domains/IdleDeathGamble.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/Characters/CurseUsers/CursedSpirits/CursedSpirit.h"
#include "code/header/GameManagement/Utils.h"




Hakari::Hakari():Sorcerer(650.0, 5500.0, 75.0) {
    black_flash_chance = 15;
    domain = std::make_unique<IdleDeathGamble>();
    technique = std::make_unique<PrivatePureLoveTrain>();
    rct_skill = RCTProficiency::Absolute;
    domain_limit = 999;
    base_attack_damage = 55.0;

    char_name = "Hakari";
    name_color = "\033[38;5;157m";
}

std::unique_ptr<Character> Hakari::Clone() const {
    return std::make_unique<Hakari>();
}

void Hakari::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned! He's forced to skip his turn.", this->GetName());
        return;
    }
    auto pplt = static_cast<PrivatePureLoveTrain*>(this->GetTechnique());
    auto idg = static_cast<IdleDeathGamble*>(this->GetDomain());

    if (this->CEMoreThanMax(0.80) || !this->HPMoreThanMax(0.25) || idg->HasHitJackpot()) {
        this->SetCurrentReinforcement(200.0);
    }
    else if (this->CEMoreThanMax(0.60)) {
        this->SetCurrentReinforcement(100.0);
    }
    else if (this->CEMoreThanMax(0.45)) {
        this->SetCurrentReinforcement(50.0);
    }
    else {
        this->SetCurrentReinforcement(0.0);
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& s : bf.battlefield) {
        if (s.get() == this) continue;
        double score = s->GetCharacterHealth() / this->GetCharacterMaxHealth();

        if (s->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(s.get());
            if (curse_user->DomainActive()) {
                domain_users.push_back(curse_user);
                score += 0.50;
            }
        }
        else if (s->IsPhysicallyGifted()) {
            score += 0.25;
        }
        else if (s->IsaCursedSpirit()) {
            score += 0.35;
        }

        score += GetRandomNumber(-5, 5) * 0.01;

        if (score > best_score) {
            best_score = score;
            strongest = s.get();
        }
    }

    if (!strongest) return;

    if (idg->HasHitJackpot() && !pplt->BurntOut()) {
        pplt->UseJackpotRush(this, strongest);
        return;
    }

    if (!this->DomainActive() && !pplt->BurntOut() && !idg->HasHitJackpot()) {
        if (this->GetCharacterCE() >= idg->GetUseCost()) {
            this->ActivateDomain();
            return;
        }
    }

    if (!pplt->BurntOut()) {
        int roll = GetRandomNumber(1, 100);
        if (roll <= 60) {
            pplt->UseShutterDoors(this, strongest);
        }
        else{
            if (pplt->PlinkoUsed()) {
                pplt->UseShutterDoors(this, strongest);
                return;
            }
            pplt->UsePlinkoBalls(this, strongest);
        }
        return;
    }
    this->Attack(strongest);
}

void Hakari::TickCharacterSpecialty() {
    auto idg = static_cast<IdleDeathGamble*>(this->GetDomain());
    auto pplt = static_cast<PrivatePureLoveTrain*>(this->GetTechnique());

    if (idg->HasHitJackpot()) {
        this->SetCursedEnergyRegen(std::min(ce_regen * 50.0, 5500.0));
        this->BoostRCT();
        jackpot_tick++;
        if (jackpot_tick > 5) {
            idg->SetJackpot(false);
            jackpot_tick = 0;
            burnout_time = 0;
            technique_burnout_time = 0;
            is_strained = false;
            this->SetCursedEnergyRegen(saved_ce_regen);
            this->DisableRCT();
            std::println("{}'s Jackpot has worn off!", this->GetNameWithID());
        }
    }
    if (pplt->PlinkoUsed()) {
        pplt->TickPlinkoCooldown();
    }
}