#include "code/header/Characters/CurseUsers/CursedSpirits/CursedSpirit.h"
#include "code/header/Domains/Domain.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Techniques/Techniques.h"


CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {};

std::unique_ptr<Character> CursedSpirit::Clone() const {
    auto cs = std::make_unique<CursedSpirit>(max_health, max_cursed_energy, ce_regen);
    if (this->technique) cs->SetTechnique(this->technique->Clone());
    if (this->domain)    cs->SetDomain(this->domain->Clone());
    if (this->cursed_tool) cs->SetEquippedTool(this->cursed_tool->Clone());
    if (this->brain) cs->SetBrain(this->brain->Clone());
    cs->SetBaseDamage(this->base_attack_damage);      
    cs->SetBlackflashChance(this->black_flash_chance);
    cs->SetCharacterName(this->char_name, this->name_color);
    cs->SetPassiveRegen(this->passive_health_regen);
    cs->SetDomainLimit(this->domain_limit);
    cs->SetMaxReinforcement(this->max_ce_reinforcement);
    cs->SetMaxZoneTime(this->max_zone_time);
    cs->SetMaxDomainTime(this->max_domain_time);
    cs->SetBlackFlashMult(this->blackflash_mult);
    cs->SetMaxBurnoutTime(this->max_technique_burnout_time);
    return cs;
}

bool CursedSpirit::IsaCursedSpirit() const {
    return true;
}

void CursedSpirit::TickCharacterSpecialty() {
    this->Regen(passive_health_regen);
}

void CursedSpirit::SetPassiveRegen(double p) {
    passive_health_regen = p;
}

bool CursedSpirit::IsTransfigured() const {
    return false;
}