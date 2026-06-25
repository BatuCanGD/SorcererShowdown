#include "code/header/Domains/IdleDeathGamble.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/Techniques/Techniques.h"
#include "code/header/GameManagement/Colors.h"
#include "code/header/GameManagement/Utils.h"

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
void IdleDeathGamble::OnSureHit(CurseUser& user, Character& target) {
    if (!IsSurehitBlocked(target)) DumpInfo(&target);
    double roll = Utilities::GetRandomNumber(1.0, 239.0);
    if (!jackpot) {
        if (roll <= luck) 
        {
            std::println("\033[92m!!!!!!!!!!!JACKPOT!!!!!!!!!!\033[0m");
            total_uses--; jackpot = true; luck = std::max(luck / 10.0, 1.0); 
            Domain::ResetDomain(user, *this);
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
            "\033[92m╔══════════════════════════════════════════════════════════════════╗\n"
            "║                  IDLE DEATH GAMBLE: DOMAIN RULES                 ║\n"
            "╚══════════════════════════════════════════════════════════════════╝\033[0m\n"
            "● \033[1mThe Cost\033[0m           : The User gambles with their Cursed Energy, expending\n"
            "                        2500.0 Cursed Energy to buy into the game.\n"
            "● \033[1mThe SureHit\033[0m        : All sorcerers are immediately stunned. This represents\n"
            "                        the mandatory rule dump forced into the target's brain.\n"
            "● \033[1mThe Jackpot Roll\033[0m   : The user rolls for a Jackpot with a base probability\n"
            "                        of 1.0 in 239.0.\n"
            "● \033[1mThe Pity System\033[0m    : Every failed roll multiplies luck by 1.75. This caps\n"
            "                        at 239.0, effectively guaranteeing a Jackpot eventually.\n"
            "● \033[1mThe Jackpot Reward\033[0m : Hitting the Jackpot grants two massive buffs for 5 turns:\n"
            "                        - Infinite Cursed Energy: CE capacity and regen boosted by 50x.\n"
            "                        - Automatic Regeneration: 150.0 HP auto-heal per tick.\n"
            "● \033[1mThe Reset\033[0m          : Once the 5 turns are up, the rush ends and stats revert.\n"
            "● \033[1mPost-Jackpot Penalty\033[0m: User's luck is divided by 10.0, resetting the value.\n"
            "\033[92m═════════════════════════════════════════════════════════════════════\033[0m"
        );
        text_dumped = true;
    }
    target->SetStunState(true);
}

bool IdleDeathGamble::IsIdleDeathGamble()const {
    return true;
}