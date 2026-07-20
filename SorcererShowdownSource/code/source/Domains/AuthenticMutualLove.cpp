#include "Domains/AuthenticMutualLove.h"
#include "Characters/Character.h"

AuthenticMutualLove::AuthenticMutualLove() : Domain(600.0, 125.0, 16) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitCurseUser;
    name = "Authentic Mutual Love";
    color = "\033[95m";
    domain_cost = 500.0;
    surehit_damage = 125.0;
}
std::unique_ptr<Domain> AuthenticMutualLove::Clone() const {
    return std::make_unique<AuthenticMutualLove>(*this);
}