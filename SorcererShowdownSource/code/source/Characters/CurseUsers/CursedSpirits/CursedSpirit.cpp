#include "code/header/Characters/CurseUsers/CursedSpirits/CursedSpirit.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/Domains/Domain.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Techniques/Techniques.h"


CursedSpirit::CursedSpirit(double hp, double ce, double regen) : CurseUser(hp, ce, regen) {
    character_type = std::format("{}Cursed Spirit{}", Color::Purple, Color::Clear);
}

CursedSpirit::~CursedSpirit() = default;

bool CursedSpirit::IsaCursedSpirit() const { return true; }
bool CursedSpirit::IsTransfigured() const { return false; }
void CursedSpirit::TickCharacterSpecialty() { Regen(passive_health_regen); }
void CursedSpirit::SetPassiveRegen(double p) { passive_health_regen = p; }

std::unique_ptr<Character> CursedSpirit::Clone() const {
    auto cs = std::make_unique<CursedSpirit>(max_health, max_cursed_energy, ce_regen);
    if (technique) cs->SetTechnique(technique->Clone());
    if (domain)    cs->SetDomain(domain->Clone());
    if (cursed_tool) cs->SetEquippedTool(cursed_tool->Clone());
    if (brain) cs->SetBrain(brain->Clone());
    cs->SetBaseDamage(attack_damage);      
    cs->SetBlackflashChance(black_flash_chance);
    cs->SetCharacterName(name);
    cs->SetCharacterColor(color);
    cs->SetPassiveRegen(passive_health_regen);
    cs->SetDomainLimit(domain_limit);
    cs->SetMaxReinforcement(max_reinforcement);
    cs->SetMaxZoneTime(max_zone_time);
    cs->SetMaxDomainTime(max_domain_time);
    cs->SetBlackFlashMult(blackflash_mult);
    cs->SetMaxBurnoutTime(max_technique_burnout_time);
    return cs;
}