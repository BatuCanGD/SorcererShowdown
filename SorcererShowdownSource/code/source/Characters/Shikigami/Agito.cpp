#include "Characters/Shikigami/Agito.h"
#include "GameManagement/BattlefieldHeader.h"
#include "Characters/CurseUsers/CurseUser.h"

Agito::Agito() : Shikigami("Agito","\033[95m") {}

void Agito::PassiveSupport(CurseUser* user) {
    if (IsActive()) {
        user->Regen(passive_heal_amount);
        std::println("{} has healed {}", name, user->GetNameWithID());
    }
}

void Agito::OnShikigamiTurn(CurseUser* user, Battlefield&) {
    if (!IsActive()) return;
    if (user->GetCharacterCE() < summon_amount) {
        std::println("{} cannot maintain its support for {} due to insufficient Cursed Energy! It withdraws back into the shadows", name, user->GetNameWithID());
        Withdraw();
        return;
    }
    IncrementActiveTime();
    PassiveSupport(user);
    user->SpendCE(summon_amount);
}

bool Agito::IsAgito() const {
    return true;
}