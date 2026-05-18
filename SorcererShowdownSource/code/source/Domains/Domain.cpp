#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Utils.h"

Domain::~Domain() = default;

Domain::Domain(double hp, double as, double rn) : 
    domain_health(hp), 
    base_health(hp),
    domain_overwhelm_strength(as), 
    base_range(rn), 
    current_range(rn) {
}

bool Domain::Clashing() const {
    return clashing;
}

Domain::Refinement Domain::GetRefinement() const {
    return ref_level;
}

void Domain::SetClashState(bool a) {
    clashing = a;
}

double Domain::DomainRangeMult() const {
    return current_range / base_range;
}

std::string Domain::GetDomainName() const {
	return std::format("{}{}{}",color,name, Utilities::Color::Clear);
}

double Domain::GetDomainHealth() const {
    return domain_health;
}
double Domain::GetDomainStrength() const {
    return domain_overwhelm_strength;
}
double Domain::GetDomainRange() const {
    return current_range;
}

void Domain::DamageDomain(double damage) {
    domain_health = std::max(domain_health - damage, 0.0);
}

void Domain::ClashDomains(CurseUser& user1, CurseUser& user2) {
    Domain* d1 = user1.GetDomain(); Domain* d2 = user2.GetDomain();

    if (d1->GetRefinement() > d2->GetRefinement()) {
        std::println("{}'s domain has been overwhelmed by the more refined {}", user2.GetNameWithID(), d1->GetDomainName());
        user2.DeactivateDomain();
        d2->CollapseDomain();
        return;
    }
    else if (d1->GetRefinement() < d2->GetRefinement()) {
        std::println("{}'s domain has been overwhelmed by the more refined {}", user1.GetNameWithID(), d2->GetDomainName());
        user1.DeactivateDomain();
        d1->CollapseDomain();
        return;
    }

    if (d1->GetDomainRange() > d2->GetDomainRange()) {
        d2->DamageDomain(d1->GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d1->GetDomainName(), d2->GetDomainName());
    }
    else if (d2->GetDomainRange() > d1->GetDomainRange()) {
        d1->DamageDomain(d2->GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d2->GetDomainName(), d1->GetDomainName());
    }
    else {
        d1->DamageDomain(d2->GetDomainStrength() * 0.5);
        d2->DamageDomain(d1->GetDomainStrength() * 0.5);
        std::println("The domains of {} and {} are locked in an even struggle!", user1.GetNameWithID(), user2.GetNameWithID());
    }

    if (d1->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed", user1.GetNameWithID(), d1->GetDomainName());
        KillSetDomain(user1, *d1);
    }
    else if (d2->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed",user2.GetNameWithID(), d2->GetDomainName());
        KillSetDomain(user2, *d2);
    }
}

void Domain::KillSetDomain(CurseUser& user, Domain& domain) {
    user.DeactivateDomain();
    domain.CollapseDomain();
}

void Domain::CollapseDomain() {
    domain_health = base_health;
    clashing = false;
}

void Domain::OnSureHit(CurseUser&, Character&) {}

double Domain::GetUseCost() const {
    return domain_cost;
}

bool Domain::IsSurehitBlocked(Character& target) const {
    if (clashing){ return true; }
    if (target.IsaCurseUser()){
        auto s = static_cast<CurseUser*>(&target);
        if (s->CounterDomainActive()){
            std::println("{} protected himself from the {}'s surehit by using {}!", s->GetNameWithID(), this->GetDomainName(), s->GetCounterDomain()->GetDomainName());
            return true;
        }
        return false;
    }
    if (hit_type == HitType::HitsCurseUsers && target.IsPhysicallyGifted()) {
        std::println("{} couldn't detect {} due to their heavenly restriction\n"
                    "The domain's surehit didn't work!", GetDomainName(), target.GetNameWithID());
        return true;
    }
    return false;
}

Domain::HitType Domain::GetHitType() const {
    return hit_type;
}

bool Domain::IsDestroyed() const {
    return domain_health <= 0.0;
}
bool Domain::IsNeutralizer() const {
    return is_neutralizer;
}

bool Domain::IsIdleDeathGamble()const {
    return false;
}