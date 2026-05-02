#include "Rika.h"
#include "BattlefieldHeader.h"
#include "CurseUser.h"



Rika::Rika() : Shikigami(INT32_MAX) {
    char_name = "Rika";
    name_color = "\033[91m";
}

void Rika::OnShikigamiTurn(CurseUser* user, Battlefield& bf) {
    if (IsPartiallyActive()) {
        this->Manifest();
    }
    if (IsActivePhysically()) {
        if (active_turn_amount >= 5) {
            std::println("the queen of curses has reached her time limit\n{} trudges back into the shadows!", this->GetName());
            this->Withdraw();
            RikaCooldownRegeneration(user);
            return;
        }
        if (!value_saved) {
            SaveUserCursedEnergy(user);
        }
        user->SetMaxCursedEnergy(rika_ce_increase);
        user->SetCursedEnergyRegen(rika_regen_increase);

        ActiveTimeIncrementor();
    }
    else {
        RikaCooldownRegeneration(user);
    }
}

void Rika::RikaCooldownRegeneration(CurseUser* user) {
    if (!IsActivePhysically() && value_saved) {
        if (active_turn_amount >= 5) {
            active_cooldown--;
            if (active_cooldown <= 0) {
                active_turn_amount = 0;
                active_cooldown = 5;
            }
        }
        user->SetMaxCursedEnergy(user_ce);
        user->SetCursedEnergyRegen(user_regen);
        if (user->GetCharacterCE() > user->GetCharacterMaxCE()) {
            user->SetCursedEnergy(user->GetCharacterMaxCE());
        }
    }
}

void Rika::SaveUserCursedEnergy(CurseUser* user) {
    if (value_saved) return;
    user_ce = user->GetCharacterMaxCE();
    user_regen = user->GetCEregen();
    value_saved = true;
}