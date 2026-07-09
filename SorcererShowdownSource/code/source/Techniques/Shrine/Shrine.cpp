#include "code/header/Techniques/Shrine/ShrineTechnique.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

Shrine::Shrine() {
    name = "Shrine";
    color = "\033[31m";
    dismantle = std::make_unique<Dismantle>();
    cleave = std::make_unique<Cleave>();
}

Shrine::Shrine(const Shrine& other) : Technique(other) {
    cleave = std::make_unique<Cleave>();
    dismantle = std::make_unique<Dismantle>();
}

Dismantle* Shrine::GetDismantle() const{
    return dismantle.get();
}
Cleave* Shrine::GetCleave() const{
    return cleave.get();
}

void Shrine::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("You cannot use your innate technique due to domain amplification!");
        return;
    }
    if (chant >= ChantLevel::One) {
        std::println("1 - Use Dismantle | 2 - Use Cleave | 3 - Use Spiderwebbed Cleave");
    }else{
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }

    std::print("=> ");
    int choice = Utilities::GetInput<int>();
   
    switch (choice) {
    case 1:
        dismantle->UseTechnique(user, target, bf, chant);
        break;
    case 2:
        cleave->UseTechnique(user, target, bf, chant);
        break;
    case 3:
        cleave->GetSpiderwebCleave()->UseTechnique(user, target, bf, chant);
        break;
    default:
        std::println("Invalid Input");
    }
}

void Shrine::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("Chant level: [{}]", GetStringChantLevel());
    std::println("1 - Chant");
    std::print("=> ");
    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1:
        Chant();
        break;
    default:
        std::println("Invalid Input");
    }
}

void Shrine::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("{}\"Scale of the Dragon\"{}",Color::BrightRed,Color::Clear);
        chant = ChantLevel::One;
    }
    else if (chant == ChantLevel::One) {
        std::println("{}\"Recoil.\"{}", Color::BrightRed, Color::Clear);
        chant = ChantLevel::Two;
    }
    else if (chant == ChantLevel::Two) {
        std::println("{}\"Twin Meteors.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Three;
    }
    else if (chant == ChantLevel::Three) {
        std::println("{}\"Silence. Desolation. Anatomy of Ruin.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Four;
    }
    else {
        std::println("The technique is already at maximum output. Sever them!");
    }
}

bool Shrine::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (Utilities::GetRandom(1, 30) >= 25) {
        cleave->UseTechnique(user, target, bf, chant);
        return true;
    }
    Specials* sp = user->GetSpecial();
    if (sp && sp->IsWorldCuttingSlash() && sp->CheckSpecial(user)) {
        if (chant == ChantLevel::Four) {
            sp->UseSpecial(user, target, bf);
            return true;
        }
    }
    if (Utilities::GetRandom(1, 10) >= 6 || (sp && sp->CheckSpecial(user))) {
        Chant();
        return true;
    }
    else {
        dismantle->UseTechnique(user, target, bf, chant);
        return true;
    } 
}

std::unique_ptr<Technique> Shrine::Clone() const {
    return std::make_unique<Shrine>(*this);
}

bool Shrine::IsShrine() const {
    return true;
}