#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/Specials/Specials.h"
#include "code/header/Domains/Domain.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"




Sorcerer::Sorcerer(double hp, double ce, double re) : CurseUser(hp, ce, re) {}
Sorcerer::~Sorcerer() = default;

void Sorcerer::SetSixEyes(bool t) {
    six_eyes = t;
}

bool Sorcerer::HasSixEyes() const {
    return six_eyes;
}

std::unique_ptr<Character> Sorcerer::Clone() const {
    auto s = std::make_unique<Sorcerer>(max_health, max_cursed_energy, ce_regen);
    if (this->technique) s->SetTechnique(this->technique->Clone());
    if (this->domain)    s->SetDomain(this->domain->Clone());
    if (this->special)   s->SetSpecial(this->special->Clone());
    if (this->cursed_tool) s->SetEquippedTool(this->cursed_tool->Clone());
    if (this->brain) s->SetBrain(this->brain->Clone());
    s->SetCharacterName(this->char_name, this->name_color);
    s->SetSixEyes(this->six_eyes);
    s->SetBaseDamage(this->base_attack_damage); 
    s->SetBlackflashChance(this->black_flash_chance);
    s->SetDomainLimit(this->domain_limit);
    for (const auto& tool : this->inventory_curse) {
        if (tool) s->AddToolToInventory(tool->Clone());
    }
    return s;
}

void Sorcerer::SpendCE(double ce) {
    double efficiency = 1.0;
    if (HasSixEyes()) {
        efficiency = 0.3;
        if (technique && technique->GetStatus() == Technique::Status::BurntOut) {
            efficiency = 0.85; 
        }
    }
    cursed_energy = std::max(cursed_energy - (ce * efficiency), 0.0);
}

void Sorcerer::DisableRCT() {
    rct_state = ReverseCT::Disabled;
}

void Sorcerer::EnableRCT() {
    rct_state = ReverseCT::Active;
}

void Sorcerer::BoostRCT() {
    rct_state = ReverseCT::Overdrive;
}

std::string Sorcerer::GetRCTstatus() const {
    switch (rct_state) {
    case ReverseCT::Disabled: return "\033[31mDisabled\033[0m";
    case ReverseCT::Active: return "\033[33mActive\033[0m";
    case ReverseCT::Overdrive: return "\033[32mOverdrive\033[0m";
    default: return "\033[2;90mDisabled\033[0m";
    }
}

bool Sorcerer::HasRCT() const{
    return rct_skill != RCTProficiency::None;
}

double Sorcerer::GetRCTHeal() const {
    switch (GetRCTProficiency()) {
    case RCTProficiency::Crude: return 60.0;
    case RCTProficiency::Adept: return 85.0;
    case RCTProficiency::Expert: return 100.0;
    case RCTProficiency::Absolute: return 150.0;
    default: return 0.0;
    }
}
double Sorcerer::GetRCTCost() const {
    switch (GetRCTProficiency()) {
    case RCTProficiency::Crude: return 75.0;
    case RCTProficiency::Adept: return 125.0;
    case RCTProficiency::Expert: return 180.0;
    case RCTProficiency::Absolute: return 275.0;
    default: return 0.0;
    }
}
Sorcerer::RCTProficiency Sorcerer::GetRCTProficiency() const {
    return rct_skill;
}

void Sorcerer::UseRCT() {
    if (this->GetCharacterHealth() >= this->GetCharacterMaxHealth()) {
        return;
    }
    if (rct_state == ReverseCT::Active) {
        this->Regen(GetRCTHeal());
        this->SpendCEdirect(GetRCTCost());
    }
    else if (rct_state == ReverseCT::Overdrive) {
        this->Regen(GetRCTHeal() * 2);
        this->SpendCEdirect(GetRCTCost() * 2 );
    }
}

void Sorcerer::SetRCTProficiency(std::string str) {
    if (str == "None") {
        rct_skill = RCTProficiency::None;
    }
    else if (str == "Crude") {
        rct_skill = RCTProficiency::Crude;
    }
    else if (str == "Adept") {
        rct_skill = RCTProficiency::Adept;
    }
    else if (str == "Expert") {
        rct_skill = RCTProficiency::Expert;
    }
    else if (str == "Absolute") {
        rct_skill = RCTProficiency::Absolute;
    }
    else {
        rct_skill = RCTProficiency::Adept;
    }
}

bool Sorcerer::CanBeHit() const {
    return true;
}

bool Sorcerer::IsaSorcerer()const {
    return true;
}