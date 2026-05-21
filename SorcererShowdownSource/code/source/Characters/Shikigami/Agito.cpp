#include "code/header/Characters/Shikigami/Agito.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"



Agito::Agito() : Shikigami(150.0) {
    name = "Agito";
    color = "\033[95m";
}

void Agito::PassiveSupport(CurseUser* user) {
    if (IsActive()) {
        user->Regen(passive_heal_amount);
        std::println("{} has healed {}",this->GetName(), user->GetNameWithID());
    }
}

void Agito::OnShikigamiTurn(CurseUser* user, Battlefield&) {
    if (!IsActive()) {
        this->Regen(shadow_health_regen);
        return;
    }
    if (user->GetCharacterCE() < summon_amount) {
        std::println("{} cannot maintain its support for {} due to insufficient Cursed Energy! It withdraws back into the shadows",this->GetName(), user->GetNameWithID());
        this->Withdraw();
        return;
    }
    ActiveTimeIncrementor();
    PassiveSupport(user);
    user->SpendCE(summon_amount);
}

bool Agito::IsAgito() const {
    return true;
}