#include "code/header/Techniques/Techniques.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Utils.h"

Technique::~Technique() = default;

void Technique::Set(Status s) {
    state = s;
}

Technique::Status Technique::GetStatus() const {
    return state;
}

Technique::ChantLevel& Technique::GetChantLevel() {
    return chant;
}

std::string Technique::GetTechniqueName() const {
    return std::format("{}{}{}",color,name,Utilities::Color::Clear);
}
std::string Technique::GetTechniqueSimpleName() const {
    return name;
}

void Technique::Chant() {
    switch(chant){
        case ChantLevel::Zero: {
            std::println("You chant once");
            chant = ChantLevel::One;
            break;
        }
        case ChantLevel::One: {
            std::println("You chant twice");
            chant = ChantLevel::Two;
            break;
        }
        case ChantLevel::Two: {
            std::println("You chant three times");
            chant = ChantLevel::Three;
            break;
        }
        case ChantLevel::Three: {
            std::println("You chant four times");
            chant = ChantLevel::Four;   
            break;
        }
        case ChantLevel::Four:{
            std::println("Your technique is at maximum output, use it now!");
            break;
        }
    }
}

void Technique::TechniqueSetting(CurseUser*, Battlefield&) {} // no idea what to do for this

double Technique::GetTechniqueOutput() const {
    switch (state) {
    case Status::Usable:      return 1.0;
    case Status::DomainBoost: return 2.0;
    case Status::BurntOut:    return 0.35;
    default:                  return 1.0;
    }
}

bool Technique::FullyChanted()const {
    return chant == ChantLevel::Four;
}

std::string Technique::GetStringStatus() const {
    switch (state) {
	case Status::Usable:      return "\033[32mUsable\033[0m";
	case Status::DomainBoost: return "\033[33mOutput Boosted\033[0m";
	case Status::BurntOut:    return "\033[31mBurnt Out\033[0m";
    default:                  return "\033[2;90mUnknown State\033[0m";
    }
}

bool Technique::Boosted() const {
    return state == Status::DomainBoost;
}
bool Technique::Usable() const {
    return state == Status::Usable;
}
bool Technique::BurntOut() const {
    return state == Status::BurntOut;
}

double Technique::GetChantPower()const {
    return 1.0 + (static_cast<int>(chant) * 0.5);
}

void Technique::TechniqueMenu(CurseUser*, Character*, Battlefield&) {}
bool Technique::AutoTechniqueUse(CurseUser*, Character*, Battlefield&) {
    return false;
}
std::unique_ptr<Technique> Technique::Clone() const{
    return nullptr;
}

std::string Technique::GetStringChantLevel() const {
    switch (chant) {
    case ChantLevel::Zero: return "\033[2;90mZero Chants\033[0m";
    case ChantLevel::One: return "\033[31mOne Chant\033[0m";
    case ChantLevel::Two: return "\033[33mTwo Chants\033[0m";
    case ChantLevel::Three: return "\033[32mThree Chants\033[0m";
    case ChantLevel::Four: return "\033[35mMax Chants\033[0m";
    default: return "\033[31mOne Chant\033[0m";
    }
}

bool Technique::IsShrine() const {
    return false;
}

bool Technique::IsLimitless() const { 
    return false; 
}

bool Technique::IsInfinityActive() const {
    return false;
}

bool Technique::IsCopy() const {
    return false;
}