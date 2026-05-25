#include "code/header/BindingVows/Overtime.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

Overtime::Overtime(){
    name = "Overtime";
    description = "Reduces the cost of using reinforcement in exchange for the user having less room to reinforce";
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
    if (bvs == VowStatus::Active){
        if (!saved) SaveCharacterData(user);
        UseBindingVow();
    }else if (saved){
        parent->SetMaxReinforcement(saved_max_reinforcement);
        parent->SetReinforcementCostMult(saved_reinforcement_cost);
        saved = false;
    }
}