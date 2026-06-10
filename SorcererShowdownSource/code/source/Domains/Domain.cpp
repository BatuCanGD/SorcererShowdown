#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Characters/Character.h"
#include "code/header/GameManagement/Utils.h"

Domain::~Domain() = default;

Domain::Domain(double hp, double as, int rn) : 
    domain_health(hp), 
    saved_health(hp),
    domain_strength(as), 
    range(rn) {
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
    if (d1->GetRefinement() < d2->GetRefinement()) {
        std::println("{}'s domain has been overwhelmed by the more refined {}", user1.GetNameWithID(), d2->GetDomainName());
        user1.DeactivateDomain();
        d1->CollapseDomain();
        return;
    }

    ResolveRange(*d1, *d2, user1, user2);

    if (d1->IsDestroyed() && d2->IsDestroyed()){
        std::println("Both domains have been shattered at the same time under each others pressure!");
        ResetDomain(user1, *d1);
        ResetDomain(user2, *d2);
    }
    else if (d1->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed", user1.GetNameWithID(), d1->GetDomainName());
        ResetDomain(user1, *d1);
    }
    else if (d2->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed",user2.GetNameWithID(), d2->GetDomainName());
        ResetDomain(user2, *d2);
    }
}

void Domain::ResolveRange(Domain& d1, Domain& d2, CurseUser& user1, CurseUser& user2) {
    if (d1.GetDomainRange() > d2.GetDomainRange()) {
        d2.DamageDomain(d1.GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d1.GetDomainName(), d2.GetDomainName());
    }
    else if (d2.GetDomainRange() > d1.GetDomainRange()) {
        d1.DamageDomain(d2.GetDomainStrength());
        std::println("{} is overwhelming {}'s barrier!", d2.GetDomainName(), d1.GetDomainName());
    }
    else {
        d1.DamageDomain(d2.GetDomainStrength() * 0.5);
        d2.DamageDomain(d1.GetDomainStrength() * 0.5);
        std::println("The domains of {} and {} are locked in an even struggle!", user1.GetNameWithID(), user2.GetNameWithID());
    }
}

bool Domain::IsSurehitBlocked(Character& target) const {
    if (target.IsaCurseUser()){
        auto s = static_cast<CurseUser*>(&target);
        if (s->CounterDomainActive()){
            std::println("{} protected himself from the {}'s surehit by using {}!", s->GetNameWithID(), GetDomainName(), s->GetCounterDomain()->GetDomainName());
            return true;
        }
        return false;
    }
    if (hit_type == HitType::HitCurseUser && target.IsPhysicallyGifted()) {
        std::println("{} couldn't detect {} due to their heavenly restriction\n"
                    "The domain's surehit didn't work!", GetDomainName(), target.GetNameWithID());
        return true;
    }
    return false;
}

void Domain::SetDomainType(const std::string& type){
    if (type == "Hits Everyone") hit_type = HitType::HitAll;
    else if (type == "Hits Soul") hit_type = HitType::HitAllSoul;
    else hit_type = HitType::HitCurseUser;
}

void Domain::SetRefinement(const std::string& n){
    if (n == "Unstable") ref_level = Refinement::Unstable;
    else if (n == "Crude") ref_level = Refinement::Crude;
    else if (n == "Absolute") ref_level = Refinement::Absolute;
    else ref_level = Refinement::Refined;
}

void Domain::ResetDomain(CurseUser& user, Domain& domain) {
    user.DeactivateDomain();
    domain.CollapseDomain();
}

void Domain::CollapseDomain() {
    domain_health = saved_health;
}

void Domain::OnSureHit(CurseUser&, Character& target) {
    if (IsSurehitBlocked(target)) return;
    if (is_stunning) {
        target.SetStunState(true);
        std::print("{} has been stunned!", target.GetNameWithID()); 
    }
    if (hit_type == Domain::HitType::HitAllSoul) { 
        target.DamageBypassAll(surehit_damage); 
    }
    else { 
        target.DamageBypass(surehit_damage); 
    }
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), GetDomainName());
}

void Domain::SetDomainStun(bool b){ is_stunning = b; }
void Domain::SetDomainHealth(double h){ domain_health = h; saved_health = h; }
void Domain::SetDomainName(const std::string& s){ name = s; }
void Domain::SetDomainColor(const std::string& s){ color = s; }
void Domain::SetIfDomainNeutralizer(bool b){ is_neutralizer = b; }
void Domain::SetSurehitDamage(double d){ surehit_damage = d; }
void Domain::SetDomainCost(double c){ domain_cost = c; }
void Domain::SetDomainRange(int r){ range = r; }
void Domain::SetDomainOverwhelmStrength(double s){ domain_strength = s; }
void Domain::SetDomainUses(int i) { total_uses = i; }
void Domain::IncrementUses() { total_uses++; }


int Domain::GetDomainUses() const { return total_uses; }
int Domain::GetDomainRange() const { return range; }
std::string Domain::GetDomainName() const { return std::format("{}{}{}",color,name, Utilities::Color::Clear); }
std::string Domain::GetDomainSimpleName() const { return name; }
double Domain::GetDomainHealth() const { return domain_health; }
double Domain::GetDomainStrength() const { return domain_strength; }
double Domain::GetUseCost() const { return domain_cost; }
bool Domain::IsDestroyed() const { return domain_health <= 0.0; }
bool Domain::IsNeutralizer() const { return is_neutralizer; }
bool Domain::IsIdleDeathGamble()const { return false; }
Domain::HitType Domain::GetHitType() const { return hit_type; }
Domain::Refinement Domain::GetRefinement() const { return ref_level; }

std::unique_ptr<Domain> Domain::Clone() const {
    return std::make_unique<Domain>(*this);
}
