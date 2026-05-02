#include "Techniques.h"
#include "CurseUser.h"
#include "Character.h"
#include "Utils.h"



void Technique::Set(Status s) {
    state = s;
}

Technique::Status Technique::GetStatus() const {
    return state;
}

std::string Technique::GetTechniqueName() const {
    return std::format("{}{}{}",tech_color,tech_name,Color::Clear);
}
std::string Technique::GetTechniqueSimpleName() const {
    return tech_name;
}

void Technique::Chant() {}
void Technique::TechniqueSetting(CurseUser*, Battlefield&) {}

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

double Technique::CalculateDamage(CurseUser* user, double cost) const {
    double multiplier = GetTechniqueOutput();
    double currentCE = user->GetCharacterCE();

    if (currentCE < cost) {
        std::println("Insufficient Cursed Energy! Output weakened.");
        user->SpendCE(currentCE); 
        return currentCE * multiplier;
    }
    user->SpendCE(cost);
    return cost * multiplier;
}

double Technique::GetChantPower()const {
    return 1.0 + (static_cast<int>(this->chant) * 0.50);
}

std::string Technique::GetStringChantLevel() const {
    switch (chant) {
    case ChantLevel::Zero: return "\033[2;90mZero\033[0m";
    case ChantLevel::One: return "\033[31mOne\033[0m";
    case ChantLevel::Two: return "\033[33mTwo\033[0m";
    case ChantLevel::Three: return "\033[32mThree\033[0m";
    case ChantLevel::Four: return "\033[35mMax\033[0m";
    default: return "One";
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