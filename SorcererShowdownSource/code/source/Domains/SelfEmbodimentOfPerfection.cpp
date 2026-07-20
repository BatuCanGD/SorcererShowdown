#include "Domains/SelfEmbodimentOfPerfection.h"
#include "Characters/Character.h"

SelfEmbodimentOfPerfection::SelfEmbodimentOfPerfection() : Domain(500.0, 100.0, 16) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitCurseUserSoul;
    name = "Self Embodiment Of Perfection";
    color = "\033[35m";
    domain_cost = 500.0;
    surehit_damage = 150.0;
}
std::unique_ptr<Domain> SelfEmbodimentOfPerfection::Clone() const {
    return std::make_unique<SelfEmbodimentOfPerfection>(*this);
}