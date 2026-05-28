#include "code/header/BindingVows/CursedEnergySacrifice.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

CursedEnergySacrifice::CursedEnergySacrifice(){
    name = "Cursed Energy Sacrifice";
    color = "\033[36m";
    description = "Cut your cursed energy reserves in half in exchange for doubled Cursed Energy regeneration";
}

void CursedEnergySacrifice::SaveCharacterData(CurseUser* user) {
    parent = user;
    saved_regen = user->GetCursedEnergyRegen();
    saved_ce = user->GetCharacterMaxCE();
    saved = true;
}
void CursedEnergySacrifice::UseBindingVow() {
    parent->SetCursedEnergyRegen(saved_regen * 2);
    parent->SetMaxCursedEnergy(saved_ce * 0.5);
    applied = true;
}
void CursedEnergySacrifice::TickVow(CurseUser* user) {
    if (!saved) SaveCharacterData(user);
    if (!applied) UseBindingVow();
    if (set_for_removal){
        parent->SetCursedEnergyRegen(saved_regen);
        parent->SetCursedEnergy(saved_ce);
    }
}
std::unique_ptr<BindingVow> CursedEnergySacrifice::Clone() const {
    return std::make_unique<CursedEnergySacrifice>(*this);
}