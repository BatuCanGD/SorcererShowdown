#include "code/header/BindingVows/BareHanded.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

BareHanded::BareHanded(){
    name = "Bare-Handed";
    color = "\033[31m";
    description = "Consumes All cursed energy to massively amplify physical strikes";
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
    parent->SetMaxCursedEnergy(1.0);
    parent->SetBaseDamage(saved_damage * 3.0);
    applied = true;
}
void BareHanded::TickVow(CurseUser* user) {
    if (!saved) SaveCharacterData(user);
    if (!applied) UseBindingVow();
    if (set_for_removal){
        parent->SetCursedEnergyRegen(saved_regen);
        parent->SetMaxCursedEnergy(saved_ce);
        parent->SetBaseDamage(saved_damage);
    }
}
std::unique_ptr<BindingVow> BareHanded::Clone() const {
    return std::make_unique<BareHanded>(*this);
}
