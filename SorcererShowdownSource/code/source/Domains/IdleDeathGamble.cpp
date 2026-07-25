#include "Domains/IdleDeathGamble.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "Techniques/Techniques.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

IdleDeathGamble::IdleDeathGamble() : Domain(800.0, 150.0, 16) {
    ref_level = Refinement::Refined;
    hit_type = HitType::HitAll;
    name = "Idle Death Gamble";
    color = "\033[92m";
    domain_cost = 2500.0;
}
void IdleDeathGamble::SetJackpot(bool t) {
    jackpot = t;
}
void IdleDeathGamble::DoSureHit(CurseUser& user, Character& target, bool is_blocked) {
    if (!is_blocked) DumpInfo(&target);
    double roll = Utilities::GetRandom(1.0, 239.0);
    if (!jackpot) {
        if (roll <= luck) 
        {
            std::println("\033[92m!!!!!!!!!!!JACKPOT!!!!!!!!!!\033[0m");
            total_uses = std::max(total_uses / 5, 0); 
            luck = std::max(luck / 10.0, 1.0); 
            jackpot = true;
            EndDomain(&user, EndReason::Auto);
            user.GetTechnique()->Set(Technique::Status::Usable);
        } 
        else if (roll > luck) {
            std::println("\033[31m[MISS]\033[0m {} rolled {:.2f}. (Needed {:.2f} or lower)", user.GetNameWithID(), roll, luck);
            luck = std::min(luck * 1.75, 239.0);
        }
    }
}

bool IdleDeathGamble::HasHitJackpot() const {
    return jackpot;
}

std::unique_ptr<Domain> IdleDeathGamble::Clone() const {
    return std::make_unique<IdleDeathGamble>(*this);
}

void IdleDeathGamble::DumpInfo(Character* target) {
    if (!text_dumped){
        std::println(
            "{0}╔══════════════════════════════════════════════════════════════════╗\n"
            "║                  IDLE DEATH GAMBLE: DOMAIN RULES                 ║\n"
            "╚══════════════════════════════════════════════════════════════════╝{1}\n"
            "● {0}The Cost{1}              : The User gambles with their Cursed Energy, expending\n"
            "                          2500.0 Cursed Energy to buy into the game.\n"
            "● {0}The SureHit{1}           : All sorcerers are immediately stunned. This represents\n"
            "                          the mandatory rule dump forced into the target's brain.\n"
            "● {0}The Jackpot Roll{1}      : The user rolls for a Jackpot with a base probability\n"
            "                          of 1.0 in 239.0.\n"
            "● {0}The Pity System{1}       : Every failed roll multiplies luck by 1.75. This caps\n"
            "                          at 239.0, effectively guaranteeing a Jackpot eventually.\n"
            "● {0}The Jackpot Reward{1}    : Hitting the Jackpot grants two massive buffs for 5 turns:\n"
            "                           - Infinite Cursed Energy: CE capacity and regen boosted by 50x.\n"
            "                           - Automatic Regeneration: 150.0 HP auto-heal per tick.\n"
            "● {0}The Reset{1}             : Once the 5 turns are up, the rush ends and stats revert.\n"
            "● {0}Post-Jackpot Penalty{1}  : User's luck is divided by 10.0, resetting the value.\n"
            "{0}═════════════════════════════════════════════════════════════════════{1}",
            Color::BrightGreen, Color::Clear
        );
        text_dumped = true;
    }
    target->SetStunState(true);
}

bool IdleDeathGamble::IsIdleDeathGamble()const {
    return true;
}