#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Characters/Character.h"



InfiniteVoid::InfiniteVoid() : Domain(800.0, 150.0, 16.0) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitCurseUser;
    name = "Infinite Void";
    color = "\033[34m";
    domain_cost = 1000.0;
    surehit_damage = 100.0;
}
void InfiniteVoid::OnSureHit(CurseUser&, Character& target) {
    if (IsSurehitBlocked(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    target.SetStunState(true);
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> InfiniteVoid::Clone() const {
    return std::make_unique<InfiniteVoid>(*this);
}