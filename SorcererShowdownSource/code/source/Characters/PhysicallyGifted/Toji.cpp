#include "Characters/PhysicallyGifted/Toji.h"
#include "GameManagement/BattlefieldHeader.h"
#include "CursedTools/CursedToolList.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "GameManagement/Utils.h"
#include "GameManagement/VList.h"



Toji::Toji() : PhysicallyGifted(1000.0, 900.0) {
    inventory_curse.push_back(std::make_unique<InvertedSpearofHeaven>());
    inventory_curse.push_back(std::make_unique<PlayfulCloud>());
    inventory_curse.push_back(std::make_unique<SplitSoulKatana>());
    attack_damage = 75.0;

    name = "Toji";
    color = "\033[33m";
}

std::unique_ptr<Character> Toji::Clone() const {
    return std::make_unique<Toji>();
}

void Toji::OnCharacterTurn(Battlefield& bf) {
    if (IsCharacterStunned()) {
        std::println("{} is stunned and their turn will be skipped", GetName());
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
                    score += 0.20;
                }
            }
            score += 0.10;
        }
        score += Utilities::GetRandom(-5, 5) * 0.01;
        if (score > best_score) {
            best_score = score;
            target = t.get();
        }
    }

    if (!target) {
        std::println("Can't find a proper sorcerer to fight these days...");
        return;
    }

    if (Utilities::GetRandom(1, 20) <= 10) Taunt(target);

    bool needs_spear = VList::DoINeedAmplification(target);

    const auto& inv = GetCursedTools();
    if (needs_spear) {
        if (!GetTool() || !GetTool()->IsAntiTechniqueWeapon()) {
            for (size_t i = 0; i < inv.size(); ++i) {
                if (inv[i]->IsAntiTechniqueWeapon()) {
                    CursedToolChoice(i + 1);
                    return;
                }
            }
        }
    }

    if (!GetTool() || GetTool()->IsAntiTechniqueWeapon()) {
        for (size_t i = 0; i < inv.size(); ++i) {
            if (!inv[i]->IsAntiTechniqueWeapon()) {
                CursedToolChoice(i + 1);
                return;
            }
        }
    }
    
    Attack(target);
}
