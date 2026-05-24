#include "code/header/Techniques/Limitless/Blue.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

Blue::Blue() {
    damage = 100.0;
}

bool Blue::UsedMoreThanAmount() const {
    return use_amount >= 10;
}
void Blue::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (level == Technique::ChantLevel::Four)  std::println("{}\"MAXIMUM OUTPUT: BLUE!\"{}", Utilities::Color::Blue, Utilities::Color::Clear);
    std::println("{} uses {}Blue{} on {}!", user->GetNameWithID(),Utilities::Color::Cyan,Utilities::Color::Clear, target->GetNameWithID());
    target->Damage(CalculateDamage(user, damage * user->GetTechnique()->GetChantPower())); level = Technique::ChantLevel::Zero; use_amount++;
}