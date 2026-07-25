#include "Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "CharacterCreator/AI/Aggressive.h"
#include "Specials/Specials.h"
#include "Domains/Domain.h"
#include "CursedTools/CursedTool.h"
#include "Techniques/Techniques.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "GameManagement/Utils.h"
#include "GameManagement/Colors.h"

Sorcerer::Sorcerer(double hp, double ce, double re) : CurseUser(hp, ce, re) {}
Sorcerer::~Sorcerer() = default;

std::string Sorcerer::GetType() const{
    return std::format("{}Sorcerer{}", Color::Cyan, Color::Clear);

}

void Sorcerer::SpendCE(double ce) {
    double spend_mult = GetEfficiencyMult();
    if (HasSixEyes()) {
        spend_mult = std::max(spend_mult - 0.8, 0.20);
    }
    if (technique && technique->BurntOut()) {
        spend_mult += 0.65;
    }
    cursed_energy = std::max(cursed_energy - (ce * spend_mult), 0.0);
}
std::string Sorcerer::GetRCTstatus() const  {
    if (rct_amount <= 0.0) {
        return "\033[31mDisabled\033[0m";
    }

    std::string clr = Color::Yellow;

    if (rct_amount >= 200.0) clr = Color::Purple;
    else if (rct_amount >= 100.0) clr = Color::Blue;
    else if (rct_amount >= 50.0) clr = Color::Green;

    return std::format("{}Active +{:.1f}HP each turn\033[0m", clr, rct_amount);
}

double Sorcerer::GetRCTCost() const {
    switch (rct_skill) {
    case RCTProficiency::Wasteful: return rct_amount * 3.5;
    case RCTProficiency::Crude: return rct_amount * 3.0;
    case RCTProficiency::Adept: return rct_amount * 2.5;
    case RCTProficiency::Expert: return rct_amount * 2.0;
    case RCTProficiency::Absolute: return rct_amount * 1.5;
    default: return 0.0;
    }
}
void Sorcerer::TickRCT() {
    if (health >= max_health) return;
    Regen(rct_amount);
    SpendCEBypass(GetRCTCost());
}
void Sorcerer::SetRCTAmount(double a){
    rct_amount = std::clamp(a, 0.0, 500.0);
}

void Sorcerer::SetRCTProficiency(std::string str) {
    if (str == "Wasteful"){
        rct_skill = RCTProficiency::Wasteful;
    }else if (str == "Crude") {
        rct_skill = RCTProficiency::Crude;
    }else if (str == "Expert") {
        rct_skill = RCTProficiency::Expert;
    }else if (str == "Absolute") {
        rct_skill = RCTProficiency::Absolute;
    }else {
        rct_skill = RCTProficiency::Adept;
    }
}

Sorcerer::RCTProficiency Sorcerer::GetRCTProficiency() const { return rct_skill; }
void Sorcerer::SetRCTUsability(bool b) { can_use_rct = b; }
void Sorcerer::SetSixEyes(bool t) { six_eyes = t; }
bool Sorcerer::HasRCT() const{ return can_use_rct; }
bool Sorcerer::IsaSorcerer()const { return true; }
bool Sorcerer::HasSixEyes() const { return six_eyes; }

std::unique_ptr<Character> Sorcerer::Clone() const {
    auto s = std::make_unique<Sorcerer>(max_health, max_cursed_energy, ce_regen);
    if (technique) s->SetTechnique(technique->Clone());
    if (domain)    s->SetDomain(domain->Clone());
    if (special)   s->SetSpecial(special->Clone());
    if (cursed_tool) s->SetEquippedTool(cursed_tool->Clone());
    if (brain) s->SetBrain(brain->Clone());
    s->SetCharacterName(name);
    s->SetCharacterColor(color);
    s->SetRCTUsability(can_use_rct);
    s->SetSixEyes(six_eyes);
    s->SetBaseDamage(attack_damage);
    s->SetBlackflashChance(black_flash_chance);
    s->SetDomainLimit(domain_limit);
    s->SetMaxReinforcement(max_reinforcement);
    s->SetMaxZoneTime(max_zone_time);
    s->SetBlackFlashMult(blackflash_mult);
    s->SetMaxBurnoutTime(max_technique_burnout_time);
    for (const auto& tool : inventory_curse) {
        if (tool) s->AddToolToInventory(tool->Clone());
    }
    return s;
}
