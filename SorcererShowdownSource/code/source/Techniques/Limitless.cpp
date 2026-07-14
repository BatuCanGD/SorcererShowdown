#include "code/header/Techniques/Limitless.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

Limitless::Limitless() {
    name = "Limitless";
    color = "\033[36m";
}

void Limitless::UseBlue(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) {
        std::println("{}\"MAXIMUM OUTPUT: BLUE!\"{}", Color::Blue, Color::Clear);
    } 
    std::println("{} uses {}Blue{} on {}!", user->GetNameWithID(),Color::Cyan,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateOutput(user, blu_dmg));
    chant = ChantLevel::Zero;
    use_amount++;
}

void Limitless::UsePurple(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) {
        std::println("{}\"MAXIMUM OUTPUT: PURPLE!\"{}", Color::Purple, Color::Clear);
    }
    std::println("{} uses {}Purple{} on {}!", user->GetNameWithID(),Color::Purple,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateOutput(user, prl_dmg)); 
    chant = ChantLevel::Zero;
    use_amount++;
}

void Limitless::UseRed(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four){
        std::println("{}\"MAXIMUM OUTPUT: RED!\"{}", Color::Red, Color::Clear);
    }
    std::println("{} uses {}Red{} on {}!", user->GetNameWithID(),Color::BrightRed,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateOutput(user, red_dmg)); 
    chant = ChantLevel::Zero;
    use_amount++;
}

bool Limitless::CanUseUnlimitedHollow() const{
    return use_amount >= 35;
}

void Limitless::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->AmpActive()) {
        std::println("{}You cannot use your innate technique due to domain amplification!{}", Color::Red, Color::Clear);
        return;
    }
    Sorcerer* sr = user->IsaSorcerer() ? static_cast<Sorcerer*>(user) : nullptr;
    std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");

    std::print("=> ");
    int choice = Utilities::GetInput<int>();

    switch (choice) {
    case 1:
        UseBlue(user, target);
        break;
    case 2:
        if (sr){
            if (sr->HasRCT()){
                UseRed(user, target);
            }else{
                std::println("You arent able to use Reversal Techniques");
            }
        }else{
            UseRed(user, target);
        }
        break;
    case 3:
        if (sr){
            if (sr->HasRCT()){
                UsePurple(user, target);
            }else{
                std::println("You arent able to form purple due to not having access to Reversal Techniques");
            }
        }else{
            UsePurple(user, target);
        }
        break;
    default:
        std::println("Invalid Choice");
        break;
    }
}

void Limitless::TechniqueSetting(CurseUser* user, Battlefield&) {
    std::println("Infinity Status: [{}] | Chant level: [{}]", HasInvulnerabilityBarrier() ? "\033[36mActive\033[0m" : "\033[31mInactive\033[0m", GetStringChantLevel());
    std::println("1 - Turn on Infinity | 2 - Turn off Infinity | 3 - Chant | 4 - Return");
    std::print("=> ");
    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1:
        if (user->GetCharacterCE() < user->GetCharacterMaxCE() * 0.05) {
            std::println("You do not have enough Cursed Energy to alter Infinity's state.");
            return;
        }
        else if (HasInvulnerabilityBarrier()) {
            std::println("Infinity is already active");
            return;
        }
        SetInvulnerabilityBarrier(true);
        std::println("\nInfinity has been Activated");
        break;
    case 2:
        if (!HasInvulnerabilityBarrier()) {
            std::println("Infinity is already Disabled");
            return;
        }
        SetInvulnerabilityBarrier(false);
        std::println("\nInfinity has been Deactivated");
        break;
    case 3:
        Chant();
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

bool Limitless::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (Utilities::GetRandom(1, 30) >= 20) {
        UsePurple(user, target);
        return true;
    }
    Specials* sp = user->GetSpecial();
    if (sp && sp->IsUnlimitedPurple() && sp->CheckSpecial(user)){
        sp->UseSpecial(user, target, bf);
        return true;
    }
    
    if ((Utilities::GetRandom(1, 50) >= 33) || (!FullyChanted() && (sp && sp->CheckSpecial(user)))) {
        Chant();
        return true;
    }

    if (Utilities::GetRandom(0, 1) == 1) {
        UseRed(user, target);
        return true;
    }
    else {
        UseBlue(user, target);
        return true;
    }
}

bool Limitless::IsLimitless() const {
    return true;
}

bool Limitless::HasInvulnerabilityBarrier() const {
    return Infinity;
}
void Limitless::SetInvulnerabilityBarrier(bool b) {
    Infinity = b;
}

std::unique_ptr<Technique> Limitless::Clone() const {
    return std::make_unique<Limitless>(*this);
}
