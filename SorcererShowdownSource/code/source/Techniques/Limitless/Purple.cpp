#include "code/header/Techniques/Limitless/Purple.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Techniques/Limitless/UnlimitedHollowPurple.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

Purple::Purple() {
    damage = 225.0;
    unlimited_hollow_purple = std::make_unique<UnlimitedHollowPurple>();
}
bool Purple::UsedMoreThanAmount() const {
    return use_amount >= 2;
}
void Purple::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (level == Technique::ChantLevel::Four) std::println("{}\"MAXIMUM OUTPUT: PURPLE!\"{}", Utilities::Color::Purple, Utilities::Color::Clear);
    std::println("{} uses {}Purple{} on {}!", user->GetNameWithID(),Utilities::Color::Purple,Utilities::Color::Clear, target->GetNameWithID());
    target->Damage(CalculateDamage(user, damage * user->GetTechnique()->GetChantPower())); level = Technique::ChantLevel::Zero; use_amount++;
}

UnlimitedHollowPurple* Purple::GetUnlimitedHollowPurple() const {
    return unlimited_hollow_purple.get();
}