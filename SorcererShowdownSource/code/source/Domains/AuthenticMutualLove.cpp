#include "code/header/Domains/AuthenticMutualLove.h"
#include "code/header/Characters/Character.h"



AuthenticMutualLove::AuthenticMutualLove() : Domain(600.0, 125.0, 16.0) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitsCurseUsers;

    name = "Authentic Mutual Love";
    color = "\033[95m";

    domain_cost = 500.0;
    surehit_damage = 125.0;
}
void AuthenticMutualLove::OnSureHit(CurseUser&, Character& target) {
    if (IsSurehitBlocked(target)) return;
    target.DamageBypass(surehit_damage * DomainRangeMult());
    std::println("{} got bombarded by a barrage of copied techniques inside {}!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> AuthenticMutualLove::Clone() const {
    return std::make_unique<AuthenticMutualLove>(*this);
}