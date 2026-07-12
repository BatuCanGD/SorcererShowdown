#include "code/header/Domains/Domain.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"

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

void Domain::DoSureHit(CurseUser&, Character& target, bool is_blocked) {
    if (is_blocked) return;
    if (is_stunning) {
        target.SetStunState(true);
        std::print("{} has been stunned!", target.GetNameWithID()); 
    }
    hit_type == HitType::HitAllSoul ? target.DamageBypassAll(surehit_damage) : target.DamageBypass(surehit_damage);
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), GetDomainName());
}

void Domain::OnSureHit(CurseUser& user, Character& target){
    DoSureHit(user, target, IsSurehitBlocked(target));
}

void Domain::ClashDomains(CurseUser& user1, CurseUser& user2) {
    Domain* d1 = user1.GetDomain(); 
    Domain* d2 = user2.GetDomain();

    if (d1->GetRefinement() > d2->GetRefinement()) {
        std::println("{}'s domain has been overwhelmed by the more refined {}", user2.GetNameWithID(), d1->GetDomainName());
        d2->EndDomain(&user2, EndReason::Overwhelmed);
        return;
    } else if (d1->GetRefinement() < d2->GetRefinement()) {
        std::println("{}'s domain has been overwhelmed by the more refined {}", user1.GetNameWithID(), d2->GetDomainName());
        d1->EndDomain(&user1, EndReason::Overwhelmed);
        return;
    }

    ResolveRange(*d1, *d2, user1, user2);

    if (d1->IsDestroyed() && d2->IsDestroyed()){
        std::println("Both domains have been shattered at the same time under each others pressure!");
        d1->EndDomain(&user1, EndReason::Collapsed);
        d2->EndDomain(&user2, EndReason::Collapsed);
    }
    else if (d1->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed", user1.GetNameWithID(), d1->GetDomainName());
        d1->EndDomain(&user1, EndReason::Collapsed);
    }
    else if (d2->IsDestroyed()) {
        std::println("{}'s {} has been overwhelmed and has collapsed",user2.GetNameWithID(), d2->GetDomainName());
        d2->EndDomain(&user2, EndReason::Collapsed);
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
    if (CurseUser* crs = target.IsaCurseUser() ? static_cast<CurseUser*>(&target) : nullptr){
        if (crs->GetCounter() && crs->GetCounter()->IsActive()){
            std::println("{} protected himself from the {}'s surehit by using {}!", crs->GetNameWithID(), GetDomainName(), crs->GetCounter()->GetDomainName());
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

bool Domain::IsActive() const{
    return is_active;
}

bool Domain::OnCooldown() const{
    return on_cooldown;
}

void Domain::SetDomainActivation(CurseUser* crs, bool t){
    bool is_player = crs->IsThePlayer();
    if (is_neutralizer){
        is_active = t;
        std::println("{} {} {}!", crs->GetNameWithID(),t == true ? "activated" : "deactivated", name);
        return;
    }

    if (t == true){
        if (is_active) {
            if (is_player) std::println("Your domain is already active!");
            return;
        }
        if (on_cooldown) {
            if (is_player) std::println("Your domain is on cooldown. You cannot use your domain for now");
            return;
        }
        if (total_uses >= crs->GetDomainLimit()) {
            crs->DamageBypass(50.0);
            crs->SetStunState(true);
            total_uses++;
            std::println("{}You have overused your domain! You take 50 damage and are stunned for the next turn.{}", Color::Red, Color::Clear);
            return;
        }
        is_active = true;
        total_uses++;
        std::println("\n********{}Domain Expansion{}********\n" "*******{}*******\n", Color::Purple, Color::Clear, name);
        if (auto* tech = crs->GetTechnique()) {
            tech->Set(Technique::Status::DomainBoost);
        }
        return;
    }
    if (!is_active) {
        if (is_player) std::println("Your domain is already disabled!");
        return;
    }
    cd_timer = cd_max;
    is_active = false;
}

void Domain::TickDomain(CurseUser* crs){
    if (!crs) return;

    if (is_active) {
        cd_timer++;
        crs->SpendCE(domain_cost);
        if (cd_timer == cd_max - 1){
            std::println("One turn left until {}'s {} reaches its time limit",crs->GetNameWithID() ,name);
        }
        if (cd_timer >= cd_max) {
            EndDomain(crs, EndReason::Expired);
        }
        return;
    }
    if (on_cooldown) {
        cd_timer--;
        if (cd_timer == 1){
            std::println("One turn left until {}'s {} is off cooldown", crs->GetNameWithID(), name);
        }
        if (cd_timer <= 0) {
            cd_timer = 0;
            on_cooldown = false;
        }
    }
}

void Domain::SetDomainType(std::string_view type){
    if (type == "Hits Everyone") hit_type = HitType::HitAll;
    else if (type == "Hits Soul") hit_type = HitType::HitAllSoul;
    else hit_type = HitType::HitCurseUser;
}

void Domain::SetRefinement(std::string_view n){
    if (n == "Unstable") ref_level = Refinement::Unstable;
    else if (n == "Crude") ref_level = Refinement::Crude;
    else if (n == "Absolute") ref_level = Refinement::Absolute;
    else ref_level = Refinement::Refined;
}

void Domain::EndDomain(CurseUser* crs, EndReason reason) {
    if (!crs) return;

    is_active = false;
    on_cooldown = true;
    cd_timer = cd_max;
    domain_health = saved_health;

    if (!is_neutralizer) {
        if (auto* tech = crs->GetTechnique()) {
            tech->Set(Technique::Status::BurntOut);
        }
    }

    switch (reason) {
        case EndReason::Expired:
            std::println("{}'s time limit has been reached!", name);
            break;
        case EndReason::Collapsed:
            std::println("{} has collapsed!", name);
            break;
        case EndReason::Overwhelmed:
            std::println("{} was overwhelmed!", name);
            break;
        case EndReason::Manual:
            std::println("{} was manually deactivated!", name);
            break;
        case EndReason::Auto:
            std::println("{} has shattered on its own", name);
            break;
        default:
            std::println(std::cerr, "Invalid End Reason");
    }
}

std::string Domain::GetDomainStatus()const {
    return is_active ? "\033[35mActive\033[0m" : "\033[2;90mInactive\033[0m";
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
std::string Domain::GetDomainName() const { return std::format("{}{}{}",color,name, Color::Clear); }
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
