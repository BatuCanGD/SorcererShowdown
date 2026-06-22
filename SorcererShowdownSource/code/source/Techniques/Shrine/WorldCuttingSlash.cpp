#include "code/header/Techniques/Shrine/WorldCuttingSlash.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"

WorldCuttingShrine::WorldCuttingShrine(){
    damage = 2500.0;
}
void WorldCuttingShrine::UseTechnique(CurseUser* user, Character* target, Battlefield&, Technique::ChantLevel& level) {
    if (!allowed){
        std::println("You cannot use the World Cutting Slash");
        return;
    }
    if (level != Technique::ChantLevel::Four) {
        std::println("{} hasn't completed the incantations! {}The slash fails to divide the world...{}", user->GetName(), Color::Red,Color::Clear);
        return;
    }
    std::println("{} uses the {}World Cutting Slash{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    target->DamageBypassAll(damage);
    level = Technique::ChantLevel::Zero;
}
void WorldCuttingShrine::SetAllowance(bool b){
    allowed = b;
}
bool WorldCuttingShrine::CanBeUsed() const {
    return allowed;
}