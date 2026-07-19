#include "code/header/Characters/CurseUsers/Sorcerers/Yuta.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Copy.h"
#include "code/header/Characters/Shikigami/Rika.h"
#include "code/header/Domains/AuthenticMutualLove.h"
#include "code/header/Domains/SimpleDomain.h"
#include "code/header/CursedTools/Katana.h"
#include "code/header/GameManagement/Utils.h"
#include "code/header/GameManagement/VList.h"
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
    ce_efficiency = CEfficiency::Wasteful;

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
    auto* copy = GetTechnique();
    Shikigami* rika = ChooseShikigami(0);

    bool needs_rika = !HPMoreThanMax(0.50) || !CEMoreThanMax(0.20);

    if (needs_rika) {
        if (!(rika->GetActiveTime() >= 5) && !rika->IsActive()) {
            std::println("Come, Rika.");
            rika->Manifest();
        }
    }
    if (rika->IsActive()){
        SetCurrentReinforcement(500.0);
    }else if (CEMoreThanMax(0.70)) {
        SetCurrentReinforcement(200.0 + Utilities::GetRandom(0.0, 150.0));
    }else if(CEMoreThanMax(0.35)){
        SetCurrentReinforcement(100.0 + Utilities::GetRandom(0.0, 100.0));
    }else if(CEMoreThanMax(0.06)){
        SetCurrentReinforcement(50.0 + Utilities::GetRandom(0.0, 50.0));
    }else{
        SetCurrentReinforcement(Utilities::GetRandom(0.0, 50.0));
    }

    if (rika->IsActive()){
        rct_amount = 500.0;
    }else if(!HPMoreThanMax(0.20)){
        rct_amount = 400.0 + Utilities::GetRandom(-200.0, 100.0);
    }else if (!HPMoreThanMax(0.70)){
        rct_amount = 200.0 + Utilities::GetRandom(-100.0, 100.0);
    }else{
        rct_amount = 0.0;
    }

    double best_score = -1.0;
    Character* strongest = nullptr;
    std::vector<CurseUser*> domain_users;

    for (const auto& s : bf.battlefield) {
        if (s.get() == this || s->GetCharacterHealth() <= 0.0) continue;
        double score = s->GetCharacterHealth() / s->GetCharacterMaxHealth();

        if (s->IsaCurseUser()) {
            auto curse_user = static_cast<CurseUser*>(s.get());
            if (curse_user->GetDomain() && curse_user->GetDomain()->IsActive()) {
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

        score += Utilities::GetRandom(-5.0, 5.0) * 0.01;

        if (score > best_score || !strongest) {
            best_score = score;
            strongest = s.get();
        }
    }

    if (Utilities::GetRandom(1, 20) <= 4) Taunt(strongest);

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
        if (d_size == 0 && Utilities::GetRandom<int>(1, 100) >= 35){
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
    
    SetAmplification(VList::DoINeedAmplification(strongest));

    bool can_use_tech = !copy->BurntOut() && !AmpActive();

    if (can_use_tech) {
        if (copy->AutoTechniqueUse(this, strongest, bf)) return;
    }
    Attack(strongest);
}