#include "code/header/Characters/PhysicallyGifted/Toji.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/CursedTools/CursedToolList.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



Toji::Toji() : PhysicallyGifted(1000.0, 450.0) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());

    char_name = "Toji Fushiguro";
    name_color = "\033[33m";
}

std::unique_ptr<Character> Toji::Clone() const {
    return std::make_unique<Toji>();
}

void Toji::OnCharacterTurn(Character*, Battlefield& bf) {
    if (this->IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", this->GetName());
        return;
    }

    Character* target = nullptr;
    double best_score = -1.0;

    for (const auto& t : bf.battlefield) {
        if (t.get() == this || t->GetCharacterHealth() <= 0.0) continue;
        
        double hp_ratio = t->GetCharacterHealth() / t->GetCharacterMaxHealth();
        double score = 1.0 - hp_ratio;

        if (t.get()->IsaCurseUser()) {
            auto* crs = static_cast<CurseUser*>(t.get());
            if (auto* tech = crs->GetTechnique()) {
                if (tech->IsLimitless()) {
                    score += 0.15;
                }
            }
        }
        score += Utilities::GetRandomNumber(-5, 5) * 0.01;
        if (score > best_score) {
            best_score = score;
            target = t.get();
        }
    }

    if (!target) {
        std::println("Can't find a proper sorcerer to fight these days...");
        return;
    }

    int tntroll = Utilities::GetRandomNumber(1, 20);
    if (tntroll <= 10) {
        this->Taunt(target);
    }

    bool needs_spear = false;
    if (target->IsaCurseUser()) {
        auto crs = static_cast<CurseUser*>(target);
        if (auto* tech = crs->GetTechnique()) {
            if (tech->IsLimitless() && tech->IsInfinityActive()) needs_spear = true;
        }
    }
    const auto& inv = this->GetCursedTools();
    if (needs_spear) {
        if (!this->GetTool() || !this->GetTool()->IsAntiTechniqueWeapon()) {
            for (size_t i = 0; i < inv.size(); ++i) {
                if (inv[i]->IsAntiTechniqueWeapon()) {
                    this->CursedToolChoice(i + 1);
                    return;
                }
            }
        }
    }
    else {
        if (!this->GetTool() || this->GetTool()->IsAntiTechniqueWeapon()) {
            for (size_t i = 0; i < inv.size(); ++i) {
                if (!inv[i]->IsAntiTechniqueWeapon()) {
                    this->CursedToolChoice(i + 1);
                    return;
                }
            }
        }
    }
    this->Attack(target);
}
