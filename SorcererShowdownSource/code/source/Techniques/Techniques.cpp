#include "Techniques/Techniques.h"
#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "GameManagement/Colors.h"

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
    return std::format("{}{}{}",color,name,Color::Clear);
}
std::string Technique::GetTechniqueSimpleName() const {
    return name;
}

double Technique::CalculateOutput(CurseUser* user, double output) const {
    double multiplier = this->GetTechniqueOutput() * (1 + static_cast<int>(chant)) ;
    double currentCE = user->GetCharacterCE();
    if (currentCE < output) {
        std::println("Insufficient Cursed Energy! Output weakened.");
        user->SpendCE(currentCE); 
        return currentCE * multiplier;
    }
    user->SpendCE(output);
    return output * multiplier;
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

bool Technique::TechniqueSetting(CurseUser*, Battlefield&) { 
    return false; 
}

bool Technique::HasInvulnerabilityBarrier() const { 
    return false; 
}

void Technique::SetInvulnerabilityBarrier(bool){}

void Technique::TickTechnique(CurseUser* crs){
    if (!crs) return;
    Domain* domain = crs->GetDomain();
    if (BurntOut() || (Boosted() && (!domain || !domain->IsActive()))) {
        cd_timer++;
    }else{
        cd_timer = 0;
    }
    if (cd_timer >= cd_max){
        state = Status::Usable;
        cd_timer = 0;
    }
    TickTechniqueSpecialty(crs);
}

void Technique::TickTechniqueSpecialty(CurseUser*) {}

void Technique::InvulnerabilityNerf(CurseUser* user) {
    if (!HasInvulnerabilityBarrier()) return;
    double barrier_cost = 250.0;
    if (BurntOut()) {
        SetInvulnerabilityBarrier(false);
        std::println("{}{}'s protective barrier shatters due to technique burnout!{}", Color::Cyan, user->GetNameWithID(), Color::Clear);
    }
    else if (user->GetCharacterCE() < barrier_cost) {
        SetInvulnerabilityBarrier(false);
        std::println("{}{}'s concentration wavers due to low CE!{}{} Their protective barrier is deactivated.{}",Color::Red,user->GetNameWithID(),Color::Clear,Color::Cyan,Color::Clear);
    }
    else{
        user->SpendCE(barrier_cost);
    }
}

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

bool Technique::TechniqueMenu(CurseUser*, Character*, Battlefield&) {
    return false;
}
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

bool Technique::IsCopy() const {
    return false;
}
