#include "code/header/Techniques/Copy.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"



Copy::Copy() {
    name = "Copy";
    color = "\033[95m";
}

std::unique_ptr<Technique> Copy::Clone() const {
    auto new_copy = std::make_unique<Copy>();

    for (const auto& tech : copied_techniques) {
        new_copy->copied_techniques.push_back(tech->Clone());
    }
    new_copy->active_copy = active_copy;
    new_copy->state = state;
    new_copy->chant = chant;
    return new_copy;
}

void Copy::Set(Status s) {
    state = s;
    for (auto& tech : copied_techniques) {
        tech->Set(s);
    }
}

void Copy::CopyFrom(CurseUser* user, CurseUser* target) {
    if (!target || !target->GetTechnique()) {
        std::println("Nothing to copy!");
        return;
    }
    if (target->IsPhysicallyGifted()) {
        std::println("{} has no cursed technique to copy!", target->GetName());
        return;
    }
    if (copied_techniques.size() >= max_copies) {
        std::println("Copy limit reached ({})!", max_copies);
        return;
    }
    if (target->GetTechnique()->IsCopy()) {
        std::println("Cannot copy from another Copy user!");
        return;
    }
    if (user->GetCharacterCE() < 500.0) {
        std::println("Not enough cursed energy to copy!");
        return;
    }
    std::string ttname = target->GetTechnique()->GetTechniqueName();
    for (const auto& tech : copied_techniques) {
        if (tech->GetTechniqueName() == ttname) {
            std::println("You have already copied this technique!");
            return;
        }
    }
    auto cloned = target->GetTechnique()->Clone();
    cloned->Set(state);
    user->SpendCE(500.0);
    std::println("Copied {}'s {}!", target->GetName(), cloned->GetTechniqueName());
    copied_techniques.push_back(std::move(cloned));
    if (!copied_techniques.empty()) {
        active_copy = copied_techniques.size() - 1; 
    }
}

void Copy::SwitchCopy(size_t index) {
    if (index >= copied_techniques.size()) {
        std::println("Invalid choice.");
        return;
    }
    active_copy = index;
    std::println("Switched to: {}", copied_techniques[active_copy]->GetTechniqueName());
}

Technique* Copy::GetActive() const {
    if (active_copy >= copied_techniques.size())
        return nullptr;
    return copied_techniques[active_copy].get();
}

void Copy::Chant() {
    if (Technique* t = GetActive()) t->Chant();
    else std::println("No technique active to chant for!");
}

void Copy::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    Technique* t = GetActive();
    if (!t) {
        std::println("No technique used! Use Technique Settings to copy or switch to one first.");
        return;
    }
    t->TechniqueMenu(user, target, bf);
}

void Copy::TechniqueSetting(CurseUser* user, Battlefield& bf) {
    std::println("=== Copy Technique Settings ===");
    std::println("Active: {}", GetTechniqueName());
    std::println("Stored copies: {}", copied_techniques.size());

    for (size_t i = 0; i < copied_techniques.size(); ++i) {
        std::println("  [{}] {}", i, copied_techniques[i]->GetTechniqueName());
    }

    std::println("1 - Copy from a target | 2 - Switch active copy | 3 - Return");
    std::print("=> ");

    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1: {
        std::println("Choose a target to copy from:");

        for (size_t i = 0; i < bf.battlefield.size(); ++i) {
            if (bf.battlefield[i].get() == user || bf.battlefield[i]->GetCharacterHealth() <= 0) continue;
            if (bf.battlefield[i].get()->IsaCurseUser()) {
                auto* sorcerer = static_cast<CurseUser*>(bf.battlefield[i].get());
                std::println("{} - {}", i, sorcerer->GetName());
            }
        }

        std::print("=> ");
        size_t tdex = Utilities::GetInput<size_t>();
        if (tdex < bf.battlefield.size() && bf.battlefield[tdex].get() != user && bf.battlefield[tdex]->GetCharacterHealth() > 0) {
            if (bf.battlefield[tdex].get()->IsaCurseUser()) {
                auto cr = static_cast<CurseUser*>(bf.battlefield[tdex].get());
                CopyFrom(user, cr);
            }
        }
        else {
            std::println("Invalid target missed!");
        }
        break;
    }
    case 2: {
        if (copied_techniques.empty()) {
            std::println("No copies to switch to.");
            break;
        }
        std::println("Enter index: ");
        size_t dex = Utilities::GetInput<size_t>();
        SwitchCopy(dex);
        break;
    }
    case 3:
        break;
    default:
        std::println("Invalid Input!");
    }
}

bool Copy::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    bool dont_copy = false;
    if (target->IsaCurseUser()) {
        auto crs = static_cast<CurseUser*>(target);
        if (!crs->GetTechnique() || 
        user->GetCharacterCE() < 500.0 || 
        crs->GetTechnique()->IsCopy() || 
        copied_techniques.size() >= max_copies) 
        {
            dont_copy = true;
        }
        std::string ttname = crs->GetTechnique()->GetTechniqueName();
        for (const auto& tech : copied_techniques) {
            if (tech->GetTechniqueName() == ttname) {
                dont_copy = true;
            }
        }
        if (!dont_copy) {
            CopyFrom(user, crs);
        }
    }
    Technique* active = GetActive();
    if (active) {
        return GetActive()->AutoTechniqueUse(user, target, bf);
    }
    return false;
}

bool Copy::IsCopy() const {
    return true;
}