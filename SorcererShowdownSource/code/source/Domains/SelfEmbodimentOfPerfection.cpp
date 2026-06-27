#include "code/header/Domains/SelfEmbodimentOfPerfection.h"
#include "code/header/Characters/Character.h"



SelfEmbodimentOfPerfection::SelfEmbodimentOfPerfection() : Domain(500.0, 100.0, 16) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitCurseUser;
    name = "Self Embodiment Of Perfection";
    color = "\033[35m";
    domain_cost = 500.0;
    surehit_damage = 150.0;
}

void SelfEmbodimentOfPerfection::DoSureHit(CurseUser&, Character& target, bool is_blocked) {
    if (is_blocked) return;
    target.DamageBypass(surehit_damage);
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), GetDomainName());
}

std::unique_ptr<Domain> SelfEmbodimentOfPerfection::Clone() const {
    return std::make_unique<SelfEmbodimentOfPerfection>(*this);
}