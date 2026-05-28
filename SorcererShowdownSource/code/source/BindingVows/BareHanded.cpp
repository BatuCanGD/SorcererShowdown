#include "code/header/BindingVows/BareHanded.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

BareHanded::BareHanded(){
    name = "Bare-Handed";
    color = "\033[31m";
    description = "Throw away techniques, cursed energy and weapons, and your fists shall become a thousand times stronger";
}

void BareHanded::SaveCharacterData(CurseUser* user) {
    parent = user;
    saved_regen = user->GetCursedEnergyRegen();
    saved_ce = user->GetCharacterMaxCE();
    saved_damage = user->GetBaseAttackDamage();
    saved = true;
}
void BareHanded::UseBindingVow() {
    parent->SetCursedEnergyRegen(0.0);
    parent->SetMaxCursedEnergy(10.0);
    parent->SetBaseDamage(saved_damage * 4.5);
    applied = true;
}
void BareHanded::TickVow(CurseUser* user) {
    if (!saved) SaveCharacterData(user);
    if (!applied) UseBindingVow();
    if (set_for_removal){
        parent->SetCursedEnergyRegen(saved_regen);
        parent->SetCursedEnergy(saved_ce);
        parent->SetBaseDamage(saved_damage);
    }
}
std::unique_ptr<BindingVow> BareHanded::Clone() const {
    return std::make_unique<BareHanded>(*this);
}