#include "code/header/Domains/InfiniteVoid.h"
#include "code/header/Techniques/Techniques.h"    
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"



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
    if (target.IsaCurseUser()){ 
        auto curse_user = static_cast<CurseUser*>(&target);
        if (curse_user->GetTechnique()){
            curse_user->GetTechnique()->Set(Technique::Status::BurntOut);
        } 
    }
    std::println("{} got hit by {}'s SureHit!", target.GetNameWithID(), this->GetDomainName());
}

std::unique_ptr<Domain> InfiniteVoid::Clone() const {
    return std::make_unique<InfiniteVoid>(*this);
}