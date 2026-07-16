#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"



InfiniteVoid::InfiniteVoid() : Domain(800.0, 150.0, 16) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitCurseUser;
    name = "Infinite Void";
    color = "\033[34m";
    domain_cost = 1000.0;
    surehit_damage = 100.0;
}
void InfiniteVoid::DoSureHit(CurseUser&, Character& target, bool is_blocked) {
    if (is_blocked) return;
    target.DamageBypass(surehit_damage);
    target.SetStunState(true);
    if (target.IsaCurseUser()){ 
        auto curse_user = static_cast<CurseUser*>(&target);
        if (auto* tech = curse_user->GetTechnique()){
            tech->Set(Technique::Status::BurntOut);
        } 
        if (auto* domain = curse_user->GetDomain()){
            domain->IncrementUses();
        }
    }
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), GetDomainName());
}

std::unique_ptr<Domain> InfiniteVoid::Clone() const {
    return std::make_unique<InfiniteVoid>(*this);
}