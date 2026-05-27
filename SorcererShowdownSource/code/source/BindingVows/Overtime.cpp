#include "code/header/BindingVows/Overtime.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

Overtime::Overtime(){
    name = "Overtime";
    description = "Reduces the cost of using reinforcement in exchange for the user having less maximum reinforcement";
}


void Overtime::SaveCharacterData(CurseUser* user) {
    parent = user;
    saved_reinforcement_cost = user->GetReinforcementCostMult();
    saved_max_reinforcement = user->GetMaxReinforcement();
    saved = true;
}
void Overtime::UseBindingVow() {
    if (!applied) { 
        parent->SetMaxReinforcement(parent->GetMaxReinforcement() * 0.5);
        parent->SetReinforcementCostMult(parent->GetReinforcementCostMult() * 0.45);
        applied = true;
    }
}
void Overtime::TickVow(CurseUser* user){
    if (!saved) SaveCharacterData(user);
    UseBindingVow();
    if (set_for_removal){
        parent->SetMaxReinforcement(saved_max_reinforcement);
        parent->SetReinforcementCostMult(saved_reinforcement_cost);
    }
}
std::unique_ptr<BindingVow> Overtime::Clone() const {
    return std::make_unique<Overtime>(*this);
}