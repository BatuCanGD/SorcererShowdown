#include "code/header/Techniques/Shrine/Dismantle.h"
#include "code/header/Techniques/Shrine/WorldCuttingSlash.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"

Dismantle::Dismantle(){
    world_cutting_slash = std::make_unique<WorldCuttingShrine>();
    damage = 100.0;
}
void Dismantle::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (level == Technique::ChantLevel::Four)  std::println("{}\"MAXIMUM OUTPUT: DISMANTLE!\"{}", Color::BrightRed, Color::Clear);
    std::println("{} uses {}Dismantle{} on {}!", user->GetNameWithID(),Color::BrightRed,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateDamage(user, damage * user->GetTechnique()->GetChantPower())); level = Technique::ChantLevel::Zero; use_amount++;
}
WorldCuttingShrine* Dismantle::GetWorldCuttingSlash() const{
    return world_cutting_slash.get();
}