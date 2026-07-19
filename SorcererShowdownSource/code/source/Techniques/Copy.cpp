#include "code/header/Techniques/Copy.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
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

bool Copy::CopyFrom(CurseUser* user, CurseUser* target) {
    if (!target || !target->GetTechnique()) {
        std::println("Nothing to copy!");
        return false;
    }
    if (target->IsPhysicallyGifted()) {
        std::println("{} has no cursed technique to copy!", target->GetName());
        return false;
    }
    if (copied_techniques.size() >= max_copies) {
        std::println("Copy limit reached ({})!", max_copies);
        return false;
    }
    if (target->GetTechnique()->IsCopy()) {
        std::println("Cannot copy from another Copy user!");
        return false;
    }
    if (user->GetCharacterCE() < copy_cost) {
        std::println("Not enough cursed energy to copy!");
        return false;
    }
    std::string_view ttname = target->GetTechnique()->GetTechniqueName();
    for (const auto& tech : copied_techniques) {
        if (tech->GetTechniqueName() == ttname) {
            std::println("You have already copied this technique!");
            return false;
        }
    }
    auto cloned = target->GetTechnique()->Clone();
    cloned->Set(state);
    user->SpendCE(copy_cost);
    std::println("Copied {}'s {}!", target->GetName(), cloned->GetTechniqueName());
    copied_techniques.push_back(std::move(cloned));
    if (!copied_techniques.empty()) {
        active_copy = copied_techniques.size() - 1; 
    }
    return true;
}

bool Copy::SwitchCopy(size_t index) {
    if (index >= copied_techniques.size()) {
        std::println("Invalid choice.");
        return false;
    }
    active_copy = index;
    std::println("Switched to: {}", copied_techniques[active_copy]->GetTechniqueName());
    return true;
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

bool Copy::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    Technique* t = GetActive();
    if (!t) {
        std::println("No technique used! Use Technique Settings to copy or switch to one first.");
        return false;
    }
    return t->TechniqueMenu(user, target, bf);
}

bool Copy::TechniqueSetting(CurseUser* user, Battlefield& bf) {
    std::println("=== Copy Technique Settings ===");
    std::println("Active copy: {}", GetTechniqueName());
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
            auto* current = bf.battlefield[i].get();
            if (!current || current == user || current->GetCharacterHealth() <= 0.0 || !current->IsaCurseUser()) continue;
            std::println("{} - {}", i, static_cast<CurseUser*>(current)->GetName());
        }

        std::print("=> ");
        size_t tdex = Utilities::GetInput<size_t>();

        if (tdex >= bf.battlefield.size()) {
            std::println("Invalid target missed!");
            return false;
        }

        auto* target = bf.battlefield[tdex].get();

        if (!target || target == user || target->GetCharacterHealth() <= 0.0) {
            std::println("Invalid target missed!");
            return false;
        }
        if (!target->IsaCurseUser()) {
            std::println("Target is not a curse user!");
            return false;
        }
        return CopyFrom(user, static_cast<CurseUser*>(target));
    }
    case 2: {
        if (copied_techniques.empty()) {
            std::println("No copies to switch to.");
            return false;
        }
        std::println("Enter index: ");
        return SwitchCopy(Utilities::GetInput<size_t>());
    }
    case 3:
        break;
    default:
        std::println("Invalid Input!");
    }
    return false;
}

bool Copy::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (target->IsaCurseUser()) {
        auto* crs = static_cast<CurseUser*>(target);
        
        bool already_copied = std::any_of(copied_techniques.begin(), copied_techniques.end(),
            [ttname = crs->GetTechnique() ? crs->GetTechnique()->GetTechniqueName() : ""](const auto& tech) {
            return tech->GetTechniqueName() == ttname;
        });

        if (crs->GetTechnique() && user->GetCharacterCE() >= 500.0 && !crs->GetTechnique()->IsCopy() && 
            copied_techniques.size() < max_copies && !already_copied) 
        {
            CopyFrom(user, crs);
        }
    }

    if (Technique* active = GetActive()) {
        return active->AutoTechniqueUse(user, target, bf);
    }
    return false;
}

bool Copy::IsCopy() const {
    return true;
}