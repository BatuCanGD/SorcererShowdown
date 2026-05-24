#include "code/header/Techniques/Shrine/SpiderwebCleave.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"

SpiderwebCleave::SpiderwebCleave(){
    damage = 25.0;
}
void SpiderwebCleave::UseTechnique(CurseUser* user, Character* target, Battlefield& bf, Technique::ChantLevel& level) {
    if (level < Technique::ChantLevel::One){
        std::println("You need a higher output to use this move");
        return;
    }
    std::println("====\"Cleave\" ******SPIDERWEB******====");
    for (const auto& c : bf.battlefield){
        if (c.get() == user) continue;
        c->SetCharacterStunState(true);
        c->Damage(damage);
        std::println("{} has been stunned, the spiderwebbed cleave has put them off balance", c->GetNameWithID());
    }
    level = Technique::ChantLevel::Zero;
}