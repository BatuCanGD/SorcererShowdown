#include "code/header/Techniques/Shrine/Shrine.h"
#include "code/header/Techniques/Shrine/Cleave.h"
#include "code/header/Techniques/Shrine/Dismantle.h"
#include "code/header/Techniques/Shrine/SpiderwebCleave.h"
#include "code/header/Techniques/Shrine/WorldCuttingSlash.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
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
    auto wcs_allowed = this->GetDismantle()->GetWorldCuttingSlash()->CanBeUsed();
    if (wcs_allowed) {
        std::println("1 - Use Dismantle | 2 - Use Cleave | 3 - Use Spiderwebbed Cleave || 4 - {}Use the World Cutting Slash{}",Utilities::Color::Red,Utilities::Color::Clear);
    }
    else if (chant < ChantLevel::One) {
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }else{
        std::println("1 - Use Dismantle | 2 - Use Cleave | 3 - Use Spiderwebbed Cleave");
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
    case 4:
        if (wcs_allowed) {
            dismantle->GetWorldCuttingSlash()->UseTechnique(user, target, bf, chant);
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
    std::println("1 - Chant");
    std::print("=> ");
    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1:
        this->Chant();
        break;
    default:
        std::println("Invalid Input");
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

bool Shrine::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (Utilities::GetRandomNumber(1, 30) >= 25) {
        cleave->UseTechnique(user, target, bf, chant);
        return true;
    }
    else {
        auto wcs_allowed = GetDismantle()->GetWorldCuttingSlash()->CanBeUsed();
        if (wcs_allowed && chant == ChantLevel::Four){
            GetDismantle()->GetWorldCuttingSlash()->UseTechnique(user, target, bf, chant);
            return true;
        }
        else {
            if (Utilities::GetRandomNumber(1, 10) >= 6 || wcs_allowed) {
                Chant();
                return true;
            }
            else {
                dismantle->UseTechnique(user, target, bf, chant);
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