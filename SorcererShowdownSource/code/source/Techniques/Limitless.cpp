#include "code/header/Techniques/Limitless.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Utils.h"



Limitless::Limitless() {
    tech_name = "Limitless";
    tech_color = "\033[36m";
}

void Limitless::UseBlue(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"MAXIMUM OUTPUT: BLUE!\"{}", Color::Blue, Color::Clear);
    println("{} uses {}Blue{} on {}!", user->GetNameWithID(),Color::Blue,Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, blue_output * GetChantPower());
    target->Damage(dmg);
    blue_used_amount++;
    chant = ChantLevel::Zero;
}

void Limitless::UseRed(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"MAXIMUM OUTPUT: RED!\"{}",Color::Red, Color::Clear);
    println("{} uses {}Red{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, red_output * GetChantPower());
    target->Damage(dmg);
    red_used_amount++;
    chant = ChantLevel::Zero;
}

void Limitless::UsePurple(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"HOLLOW PURPLE!\"{}",Color::Purple,Color::Clear);
    println("{} hits {} with a {}Hollow Purple!{}", user->GetNameWithID(), target->GetNameWithID(), Color::Purple,Color::Clear);
    double dmg = CalculateDamage(user, purple_output * GetChantPower());
    target->Damage(dmg);
    purple_used_amount++;
    chant = ChantLevel::Zero;
}

void Limitless::SetUnlimitedHollow(bool t) {
    unlimited_hollow_purple_allowed = t;
}

bool Limitless::UnlimitedHollowAllowed() const {
    return unlimited_hollow_purple_allowed;
}

void Limitless::SetInfinity(bool s) {
    Infinity = s;
}

bool Limitless::CheckInfinity() const {
    return Infinity;
}

bool Limitless::UPBlueCheck() const {
    return blue_used_amount >= 15;
}
bool Limitless::UPRedCheck() const {
    return red_used_amount >= 15;
}
bool Limitless::UPPurpleCheck() const {
    return purple_used_amount >= 5;
}

void Limitless::UseUnlimitedHollowPurple(CurseUser* user, Battlefield& bf) {
    if (up_used) {
        std::println("Unlimited hollow purple cannot be used again");
        return;
    }
    if (!this->FullyChanted()) {
        std::println("the Unlimited Hollow Purple doesnt have enough output, chant to its maximum output and potential!");
        return;
    }
    std::println("{}===== !UNLIMITED HOLLOW PURPLE! ====={}", Color::Purple, Color::Clear);
    for (const auto& s : bf.battlefield) {
        if (s.get() == user) {
            s->Damage(unlpurple_output * 0.15);
            if (s->GetCharacterHealth() <= 0.0) {
                std::println("The {}Unlimited Hollow Purple{} was too strong for {} himself",Color::Purple,Color::Clear ,s->GetNameWithID());
            }
            else {
                std::println("{} took the hit and received{} {} damage!{}",s->GetNameWithID(), Color::Red, unlpurple_output * 0.25, Color::Clear);
            }
            continue;
        }
        s->Damage(unlpurple_output);
        std::println("{} got hit by Unlimited Hollow Purple for {}{:.1f} damage!{}", s->GetNameWithID(), Color::Red, unlpurple_output , Color::Clear);
    }
    up_used = true;
    chant = ChantLevel::Zero;
}

void Limitless::InfinityNerf(CurseUser* user) {
    if (this->BurntOut()) {
        if (CheckInfinity()) {
            std::println("{}{}'s Infinity shatters due to technique burnout!{}",Color::Cyan, user->GetNameWithID(), Color::Clear);
            SetInfinity(false);
        }
        return;
    }
    if (this->CheckInfinity()) {
        double maintain_cost = 150.0;
        if (user->GetCharacterCE() < maintain_cost) {
            std::println("{}{}'s concentration wavers due to low CE!{}{} Infinity is deactivated.{}",Color::Red,user->GetNameWithID(),Color::Clear,Color::Cyan,Color::Clear);
            SetInfinity(false);
        }
        else {
            user->SpendCEdirect(maintain_cost);
        }
    }
}

void Limitless::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("{}You cannot use your innate technique due to domain amplification!{}", Color::Red, Color::Clear);
        return;
    }
    if (unlimited_hollow_purple_allowed) {
        std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple | 4 - {}Nuke the Battlefield{}",Color::Red,Color::Clear);
    }
    else {
        std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");
    }

    std::print("=> ");
    size_t choice = GetValidInput();
    if (choice == 4 && unlimited_hollow_purple_allowed) {
        UseUnlimitedHollowPurple(user, bf);
    }
    else {
        switch (choice) {
        case 1:
            UseBlue(user, target);
            break;
        case 2:
            UseRed(user, target);
            break;
        case 3:
            UsePurple(user, target);
            break;
        default:
            std::println("Invalid Input");
        }
    }
}

void Limitless::TechniqueSetting(CurseUser* user, Battlefield&) {
    std::println("Infinity Status: [{}] | Chant level: [{}]", this->CheckInfinity() ? "\033[36mActive\033[0m" : "\033[31mInactive\033[0m", this->GetStringChantLevel());
    std::println("1 - Turn on Infinity | 2 - Turn off Infinity | 3 - Chant | 4 - Return");
    std::print("=> ");
    int ch = GetValidInput();
    switch (ch) {
    case 1:
        if (user->GetCharacterCE() < 100.0) {
            std::println("You do not have enough Cursed Energy to alter Infinity's state.");
            return;
        }
        else if (CheckInfinity()) {
            std::println("Infinity is already active");
            return;
        }
        this->SetInfinity(true);
        std::println("\nInfinity has been Activated");
        break;
    case 2:
        if (!CheckInfinity()) {
            std::println("Infinity is already Disabled");
            return;
        }
        this->SetInfinity(false);
        std::println("\nInfinity has been Deactivated");
        break;
    case 3:
        this->Chant();
        break;
    case 4:
        break;
    default:
        std::println("Invalid Input! Skipping turn");
    }


}

void Limitless::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("\"{}Phase. Twilight.{}\"",Color::Cyan,Color::Clear);
        chant = ChantLevel::One;
        return;
    }
    else if (chant == ChantLevel::One) {
        std::println("\"{}Paramita. Pillars of Light.{}\"",Color::Blue,Color::Clear);
        chant = ChantLevel::Two;
        return;
    }
    else if (chant == ChantLevel::Two) {
        std::println("\"{}Nine ropes. Polarized light. Crow and Shomyo.{}\"",Color::BrightRed,Color::Clear);
        chant = ChantLevel::Three;
        return;
    }
    else if (chant == ChantLevel::Three) {
        std::println("\"{}The gap between within and without.{}\"",Color::BrightMagenta,Color::Clear);
        chant = ChantLevel::Four;
        return;
    }
    else {
        std::println("Its time to use your Technique, its not gonna get anymore stronger");
    }
}

std::unique_ptr<Technique> Limitless::Clone() const {
    return std::make_unique<Limitless>(*this);
}

bool Limitless::UnlimitedHollowUsed()const {
    return up_used;
}

void Limitless::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (GetRandomNumber(1, 30) >= 20) {
        UsePurple(user, target);
        return;
    }
    if (user->GetSpecial() && user->GetSpecial()->IsUnlimitedPurple()) {
        if (!up_used && unlimited_hollow_purple_allowed && this->FullyChanted()){
            UseUnlimitedHollowPurple(user, bf);
        }
    }
    if (GetRandomNumber(1, 50) >= 33 || (unlimited_hollow_purple_allowed && !this->FullyChanted() && !up_used)) {
        Chant();
    }
    else {
        if (GetRandomNumber(0, 1) == 1) {
            UseRed(user, target);
        }
        else {
            UseBlue(user, target);
        }
    }
}

bool Limitless::IsLimitless() const {
    return true;
}

bool Limitless::IsInfinityActive() const {
    return Infinity;
}