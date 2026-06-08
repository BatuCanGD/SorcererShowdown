#include "code/header/Domains/IdleDeathGamble.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/GameManagement/Utils.h"



IdleDeathGamble::IdleDeathGamble() : Domain(800.0, 150.0, 16) {
    ref_level = Refinement::Absolute;
    hit_type = HitType::HitAll;
    name = "Idle Death Gamble";
    color = "\033[92m";
    domain_cost = 25.0;
}
void IdleDeathGamble::SetJackpot(bool t) {
    jackpot = t;
}
void IdleDeathGamble::OnSureHit(CurseUser& user, Character& target) {
    if (jackpot) return;
    if (!info_dumped) DumpInfo();
    target.SetStunState(true);
    if (Utilities::GetRandomNumber(1, 239) <= luck) {
        std::println("\033[92m!!!!!!!!!!!JACKPOT!!!!!!!!!!\033[0m");
        total_uses--;
        jackpot = true; luck = std::max(luck / 10, 1); // hit jackpot, divide luck
        ResetDomain(user, *this); // break domain and reset stats for next use
        user.GetTechnique()->Set(Technique::Status::Usable); // make sure they can still use their technique
    } 
    else {
        luck = std::min(luck * 2, 239);
    }
    std::println("{} got stunned by {}'s SureHit!", target.GetNameWithID(), GetDomainName());
}

bool IdleDeathGamble::HasHitJackpot() const {
    return jackpot;
}

std::unique_ptr<Domain> IdleDeathGamble::Clone() const {
    return std::make_unique<IdleDeathGamble>(*this);
}

void IdleDeathGamble::DumpInfo() {
    std::println("\033[92mIDLE DEATH GAMBLE: DOMAIN RULES\033[0m\n"
        "The SureHit: (Rule Transmission) When the domain is activated, the target is immediately stunned. This represents the mandatory information dump where the rules of the game are forced into the target's brain.\n"
        "The Jackpot Roll: The user rolls for a Jackpot with a base probability of 1 in 239.\n"
        "The Pity System: (Luck Boost) Every time the user fails a roll, his luck value doubles. This continues until the value hits 239, effectively guaranteeing a Jackpot eventually.\n"
        "The Jackpot Reward: Hitting a Jackpot grants the user two massive buffs for a limited time :\n"
        "Infinite Energy: Cursed Energy regeneration is boosted by 50x.\n"
        "Automatic RCT: His Reverse Cursed Technique proficiency is set to Absolute.\n"
        "Round Duration: The Jackpot state lasts for exactly 5 turns(ticks).\n"
        "The Reset: Once the 5 turns are up, the Jackpot ends, and the user's CE regen and RCT skills revert to their base levels.\n"
        "The Post Jackpot Penalty: After a successful Jackpot, the user's luck is divided by 6, making the next immediate Jackpot significantly harder to hit.\n"
        "Standard Domain Constraints: Cost: Activating the domain costs 25.0 cursed energy.\n"
    );
    info_dumped = true;
}

bool IdleDeathGamble::IsIdleDeathGamble()const {
    return true;
}