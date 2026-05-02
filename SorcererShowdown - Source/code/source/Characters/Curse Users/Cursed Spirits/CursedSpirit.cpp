#include "CursedSpirit.h"
#include "Domain.h"
#include "CursedTool.h"
#include "Techniques.h"


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