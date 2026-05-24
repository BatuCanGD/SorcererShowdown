#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"
#include "code/header/Techniques/Limitless/Purple.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

UnlimitedHollowPurple::UnlimitedHollowPurple(){
    damage = 1500.0;
}
bool UnlimitedHollowPurple::UsedMoreThanAmount() const {
    return use_amount >= 1;
}
void UnlimitedHollowPurple::UseTechnique(CurseUser* user, Character* target, Battlefield& bf, Technique::ChantLevel& level) {
    if (!allowed){
        std::println("You cannot use unlimited hollow purple");
        return;
    }
    if (this->UsedMoreThanAmount()) {
        std::println("You arent able to conjure up another hollow nuke");
        return;
    }
    if (level != Technique::ChantLevel::Four) {
        std::println("You need to charge up Unlimited Purple by chanting!");
        return;
    }
    std::println("{}===== !UNLIMITED HOLLOW PURPLE! ====={}", Utilities::Color::Purple, Utilities::Color::Clear);
    for (const auto& s : bf.battlefield) {

        if (s.get() == user) {
            s->DamageBypass(damage * 0.15);
            double usrn = user->GetDamageReinforcement();
            if (s->GetCharacterHealth() <= 0.0) {
                std::println("{} got hit and defeated by their own {}unlimited hollow purple!{}",s->GetNameWithID() ,Utilities::Color::Purple,Utilities::Color::Clear);
            }
            else {
                std::println("{} took the hit and received{} {:.1f} damage!{}",s->GetNameWithID(), Utilities::Color::Red, (damage * 0.15) / usrn, Utilities::Color::Clear);
            }
            continue;
        }
        s->DamageBypass(damage);
        double trrn = target->GetDamageReinforcement();
        std::println("{} got hit by Unlimited Hollow Purple for {}{:.1f} damage!{}", s->GetNameWithID(), Utilities::Color::Red, damage / trrn, Utilities::Color::Clear);
    }
    use_amount++;
    level = Technique::ChantLevel::Zero;
}

void UnlimitedHollowPurple::SetAllowance(bool b){
    allowed = b;
}
bool UnlimitedHollowPurple::CanBeUsed(){
    return allowed;
}