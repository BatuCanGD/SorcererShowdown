#include "code/header/Techniques/Shrine.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Utils.h"



Shrine::Shrine() {
    tech_name = "Shrine";
    tech_color = "\033[31m";
}

void Shrine::SetWCS(bool s) {
    world_cutting_slash_allowed = s;
}

void Shrine::UseCleave(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"Maximum Output; ClEAVE!\"{}", Utilities::Color::Red, Utilities::Color::Clear);
    println("{} uses {}Cleave{} on {}!", user->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, cleave_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::UseDismantle(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) std::println("{}\"Maximum Output; DISMANTLE!\"{}", Utilities::Color::Red, Utilities::Color::Clear);
    println("{} uses {}Dismantle{} on {}!", user->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, slash_output * GetChantPower());
    target->Damage(dmg);
    chant = ChantLevel::Zero;
}
void Shrine::UseTheWorldCuttingSlash(CurseUser* user, Character* target) {
    if (this->chant != ChantLevel::Four) {
        std::println("{} hasn't completed the incantations! {}The slash fails to divide the world...{}", user->GetName(), Utilities::Color::Red,Utilities::Color::Clear);
        return;
    }
    println("{} uses the {}World Cutting Slash{} on {}!", user->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, target->GetNameWithID());
    double dmg = CalculateDamage(user, wcs_output);
    target->DamageBypass(dmg);
    chant = ChantLevel::Zero;
}

bool Shrine::WorldCuttingSlashUnlocked() const {
    return world_cutting_slash_allowed;
}

void Shrine::TechniqueMenu(CurseUser* user, Character* target, Battlefield&) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    if (world_cutting_slash_allowed) {
        std::println("1 - Use Dismantle | 2 - Use Cleave || 3 - {}Use the World Cutting Slash{}",Utilities::Color::Red,Utilities::Color::Clear);
    }
    else {
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }

    std::print("=> ");
    int choice = Utilities::GetValidInput();
   
    switch (choice) {
    case 1:
        UseDismantle(user, target);
        break;
    case 2:
        UseCleave(user, target);
        break;
    case 3:
        if (world_cutting_slash_allowed) {
            UseTheWorldCuttingSlash(user, target);
        }
        else {
            std::println("Invalid Input");
        }
        break;
    default:
        std::println("Invalid Input");
    }
}

void Shrine::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("Chant level: [{}]", this->GetStringChantLevel());
    std::println("1 - Chant | 2 - Return");
    std::print("=> ");

    int ch = Utilities::GetValidInput();
    switch (ch) {
    case 1:
        this->Chant();
        break;
    case 2:
        break;
    default:
        std::println("Invalid Input! Skipping turn");
    }
}

void Shrine::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("{}\"Scale of the Dragon\"{}",Utilities::Color::BrightRed,Utilities::Color::Clear);
        chant = ChantLevel::One;
    }
    else if (chant == ChantLevel::One) {
        std::println("{}\"Recoil.\"{}", Utilities::Color::BrightRed, Utilities::Color::Clear);
        chant = ChantLevel::Two;
    }
    else if (chant == ChantLevel::Two) {
        std::println("{}\"Twin Meteors.\"{}", Utilities::Color::Red, Utilities::Color::Clear);
        chant = ChantLevel::Three;
    }
    else if (chant == ChantLevel::Three) {
        std::println("{}\"Silence. Desolation. Anatomy of Ruin.\"{}", Utilities::Color::Red, Utilities::Color::Clear);
        chant = ChantLevel::Four;
    }
    else {
        std::println("The technique is already at maximum output. Sever them!");
    }
}

bool Shrine::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield&) {
    if (Utilities::GetRandomNumber(1, 30) >= 25) {
        UseCleave(user, target);
        return true;
    }
    else {
        if (world_cutting_slash_allowed && chant == ChantLevel::Four){
            UseTheWorldCuttingSlash(user, target);
            return true;
        }
        else {
            if (Utilities::GetRandomNumber(1, 10) >= 6 || world_cutting_slash_allowed) {
                Chant();
                return true;
            }
            else {
                UseDismantle(user, target);
                return true;
            }
        }
    }
}

std::unique_ptr<Technique> Shrine::Clone() const {
    return std::make_unique<Shrine>(*this);
}

bool Shrine::IsShrine() const {
    return true;
}