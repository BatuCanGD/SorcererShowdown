#include "MalevolentShrine.h"
#include "Character.h"



MalevolentShrine::MalevolentShrine() : Domain(1000.0, 300.0, 20.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitsEveryone;
    domain_name = "Malevolent Shrine";
    domain_color = "\033[31m";
    surehit_damage = 100.0;
    domain_cost = 750.0;
}
void MalevolentShrine::OnSureHit(CurseUser& user, Character& target) {
    if (CheckDomainSurehit(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> MalevolentShrine::Clone() const {
    return std::make_unique<MalevolentShrine>(*this);
}