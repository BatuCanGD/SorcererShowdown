#include "code/header/Domains/SelfEmbodimentOfPerfection.h"
#include "code/header/Characters/Character.h"



SelfEmbodimentOfPerfection::SelfEmbodimentOfPerfection() : Domain(500.0, 100.0, 16.0) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitsCurseUsers;
    name = "Self Embodiment Of Perfection";
    color = "\033[35m";
    domain_cost = 500.0;
    surehit_damage = 150.0;
};
void SelfEmbodimentOfPerfection::OnSureHit(CurseUser&, Character& target) {
    if (IsSurehitBlocked(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> SelfEmbodimentOfPerfection::Clone() const {
    return std::make_unique<SelfEmbodimentOfPerfection>(*this);
}