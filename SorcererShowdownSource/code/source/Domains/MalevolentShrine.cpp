#include "Domains/MalevolentShrine.h"
#include "Characters/Character.h"

MalevolentShrine::MalevolentShrine() : Domain(1000.0, 300.0, 20) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitAll;
    name = "Malevolent Shrine";
    color = "\033[31m";
    surehit_damage = 250.0;
    domain_cost = 750.0;
}
std::unique_ptr<Domain> MalevolentShrine::Clone() const {
    return std::make_unique<MalevolentShrine>(*this);
}