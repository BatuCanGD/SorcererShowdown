#include "Techniques/Limitless.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Specials/UnlimitedPurple.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

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

bool Limitless::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    Sorcerer* sr = user->IsaSorcerer() ? static_cast<Sorcerer*>(user) : nullptr;
    std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple | 4 - Chant | 5 - Return");

    std::print("=> ");
    int choice = Utilities::GetInput<int>();

    switch (choice) {
    case 1:
        UseBlue(user, target);
        return true;
    case 2:
        if (sr){
            if (sr->HasRCT()){
                UseRed(user, target);
            }else{
                std::println("You arent able to use Reversal Techniques");
                return false;
            }
        }else{
            UseRed(user, target);
        }
        return true;
    case 3:
        if (sr){
            if (sr->HasRCT()){
                UsePurple(user, target);
            }else{
                std::println("You arent able to form purple due to not having access to Reversal Techniques");
                return false;
            }
        }else{
            UsePurple(user, target);
        }
        return true;
    case 4:
        Chant();
        return true;
    case 5:
        break;
    default:
        std::println("Invalid Choice");
    }
    return false;
}

bool Limitless::TechniqueSetting(CurseUser* user, Battlefield&) {
    std::println("Infinity Status: [{}] | Chant level: [{}]\n",
        HasInvulnerabilityBarrier() ? "\033[36mActive\033[0m" : "\033[31mInactive\033[0m", 
        GetStringChantLevel()
    );
    std::println("1 - Turn on Infinity | 2 - Turn off Infinity | 3 - Chant | 4 - Return\n=> ");

    switch (Utilities::GetInput<int>()) {
    case 1:
        if (user->GetCharacterCE() < user->GetCharacterMaxCE() * 0.05) {
            std::println("You do not have enough Cursed Energy to alter Infinity's state.");
        }
        else if (HasInvulnerabilityBarrier()) {
            std::println("Infinity is already active");
        }
        SetInvulnerabilityBarrier(true);
        std::println("\nInfinity has been Activated");
        break;
    case 2:
        if (!HasInvulnerabilityBarrier()) {
            std::println("Infinity is already Disabled");
        }
        SetInvulnerabilityBarrier(false);
        std::println("\nInfinity has been Deactivated");
        break;
    case 3:
        Chant();
        return true;
    case 4:
        break;
    default:
        std::println("Invalid Input! Skipping turn");
    }
    return false;
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
