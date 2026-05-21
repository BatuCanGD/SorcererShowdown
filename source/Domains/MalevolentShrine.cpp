#include "code/header/Domains/MalevolentShrine.h"
#include "code/header/Characters/Character.h"



MalevolentShrine::MalevolentShrine() : Domain(1000.0, 300.0, 20.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitAll;
    name = "Malevolent Shrine";
    color = "\033[31m";
    surehit_damage = 100.0;
    domain_cost = 750.0;
}
void MalevolentShrine::OnSureHit(CurseUser&, Character& target) {
    if (IsSurehitBlocked(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> MalevolentShrine::Clone() const {
    return std::make_unique<MalevolentShrine>(*this);
}