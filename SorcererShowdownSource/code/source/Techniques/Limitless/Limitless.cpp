#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Techniques/Limitless/Red.h"
#include "code/header/Techniques/Limitless/Blue.h"
#include "code/header/Techniques/Limitless/Purple.h"
#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Specials/UnlimitedPurple.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"



Limitless::Limitless() {
    name = "Limitless";
    color = "\033[36m";
    red = std::make_unique<Red>();
    blue = std::make_unique<Blue>();
    purple = std::make_unique<Purple>();
}
Limitless::Limitless(const Limitless& other) : Technique(other) {
    Infinity = other.Infinity;
    red = std::make_unique<Red>();
    blue = std::make_unique<Blue>();
    purple = std::make_unique<Purple>();
}

void Limitless::SetInfinity(bool s) {
    Infinity = s;
}

bool Limitless::CheckInfinity() const {
    return Infinity;
}

void Limitless::InfinityNerf(CurseUser* user) {
    if (BurntOut()) {
        if (CheckInfinity()) {
            std::println("{}{}'s Infinity shatters due to technique burnout!{}",Color::Cyan, user->GetNameWithID(), Color::Clear);
            SetInfinity(false);
        }
        return;
    }
    if (CheckInfinity()) {
        double maintain_cost = 250.0;
        if (user->IsaSorcerer()){
            if (static_cast<Sorcerer*>(user)->HasSixEyes()){
                maintain_cost *= 0.1;
            }
        }
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
    Sorcerer* sr = user->IsaSorcerer() ? static_cast<Sorcerer*>(user) : nullptr;
    if (chant == ChantLevel::Four) {
        std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple | 4 - {}Nuke the Battlefield{}",Color::Red,Color::Clear);
    }
    else {
        std::println("1 - Use Blue | 2 - Use Red | 3 - Use Purple");
    }

    std::print("=> ");
    int choice = Utilities::GetInput<int>();
    if (choice == 4){
        if (chant == ChantLevel::Four){
            purple->GetUnlimitedHollowPurple()->UseTechnique(user, target, bf, chant);
        }
        else{
            std::println("Invalid Choice");
        }
        return;
    }
    
    switch (choice) {
    case 1:
        blue->UseTechnique(user, target, bf, chant);
        break;
    case 2:
        if (sr){
            if (sr->HasRCT()){  
                red->UseTechnique(user, target, bf, chant);
            }else{
                std::println("You arent able to use Reversal Techniques");
            }
        }else{
            red->UseTechnique(user, target, bf, chant);
        }
        break;
    case 3:
        if (sr){
            if (sr->HasRCT()){
                purple->UseTechnique(user, target, bf, chant);
            }else{
                std::println("You arent able to form purple due to not having access to Reversal Techniques");
            }
        }else{
            purple->UseTechnique(user, target, bf, chant);
        }
        break;
    default:
        std::println("Invalid Choice");
        break;
    }
}

void Limitless::TechniqueSetting(CurseUser* user, Battlefield&) {
    std::println("Infinity Status: [{}] | Chant level: [{}]", CheckInfinity() ? "\033[36mActive\033[0m" : "\033[31mInactive\033[0m", GetStringChantLevel());
    std::println("1 - Turn on Infinity | 2 - Turn off Infinity | 3 - Chant | 4 - Return");
    std::print("=> ");
    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1:
        if (user->GetCharacterCE() < user->GetCharacterMaxCE() * 0.05) {
            std::println("You do not have enough Cursed Energy to alter Infinity's state.");
            return;
        }
        else if (CheckInfinity()) {
            std::println("Infinity is already active");
            return;
        }
        SetInfinity(true);
        std::println("\nInfinity has been Activated");
        break;
    case 2:
        if (!CheckInfinity()) {
            std::println("Infinity is already Disabled");
            return;
        }
        SetInfinity(false);
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
    if (Utilities::GetRandomNumber(1, 30) >= 20) {
        purple->UseTechnique(user, target, bf, chant);
        return true;
    }
    bool up_used = purple->GetUnlimitedHollowPurple()->UsedMoreThanAmount();
    bool up_allowed = purple->GetUnlimitedHollowPurple()->CanBeUsed();
    if (user->GetSpecial() && user->GetSpecial()->IsUnlimitedPurple()) {
        if (!up_used && up_allowed && FullyChanted()){
            purple->GetUnlimitedHollowPurple()->UseTechnique(user, target, bf, chant);
            return true;
        }
    }
    if (Utilities::GetRandomNumber(1, 50) >= 33 || (up_allowed && !FullyChanted() && !up_used)) {
        Chant();
        return true;
    }
    else {
        if (Utilities::GetRandomNumber(0, 1) == 1) {
            red->UseTechnique(user, target, bf, chant);
            return true;
        }
        else {
            blue->UseTechnique(user, target, bf, chant);
            return true;
        }
    }
}

bool Limitless::IsLimitless() const {
    return true;
}

bool Limitless::IsInfinityActive() const {
    return Infinity;
}

Blue* Limitless::GetBlue() const{
    return blue.get();
}
Red* Limitless::GetRed() const{
    return red.get();
}
Purple* Limitless::GetPurple() const{
    return purple.get();
}

std::unique_ptr<Technique> Limitless::Clone() const {
    return std::make_unique<Limitless>(*this);
}