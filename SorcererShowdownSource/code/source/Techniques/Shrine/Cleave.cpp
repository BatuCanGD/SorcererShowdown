#include "code/header/Techniques/Shrine/Cleave.h"
#include "code/header/Techniques/Shrine/SpiderwebCleave.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

Cleave::Cleave(){
    spiderweb_cleave = std::make_unique<SpiderwebCleave>();
    damage = 250.0;
}
void Cleave::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (level == Technique::ChantLevel::Four)  std::println("{}\"MAXIMUM OUTPUT: CLEAVE!\"{}", Utilities::Color::Red, Utilities::Color::Clear);
    std::println("{} uses {}Cleave{} on {}!", user->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, target->GetNameWithID());
    target->Damage(CalculateDamage(user, damage * user->GetTechnique()->GetChantPower())); level = Technique::ChantLevel::Zero; use_amount++;
}
SpiderwebCleave* Cleave::GetSpiderwebCleave() const{
    return spiderweb_cleave.get();
}