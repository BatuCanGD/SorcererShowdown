#include "code/header/Techniques/Limitless/Red.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

Red::Red(){
    damage = 150.0;
}
bool Red::UsedMoreThanAmount() const {
    return use_amount >= 10;
}
void Red::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (level == Technique::ChantLevel::Four)  std::println("{}\"MAXIMUM OUTPUT: RED!\"{}", Utilities::Color::Red, Utilities::Color::Clear);
    std::println("{} uses {}Red{} on {}!", user->GetNameWithID(),Utilities::Color::BrightRed,Utilities::Color::Clear, target->GetNameWithID());
    target->Damage(CalculateDamage(user, damage * user->GetTechnique()->GetChantPower())); level = Technique::ChantLevel::Zero; use_amount++;
}