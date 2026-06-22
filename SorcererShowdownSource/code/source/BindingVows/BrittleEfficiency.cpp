#include "code/header/BindingVows/BrittleEfficiency.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"

BrittleEfficiency::BrittleEfficiency(){
    name = "Brittle Efficiency";
    color = "\033[32m";
    description = "Reduces the cost of using reinforcement in exchange for the user having less maximum reinforcement";
}
void BrittleEfficiency::SaveCharacterData(CurseUser* user) {
    parent = user;
    saved_reinforcement_cost = user->GetReinforcementCostMult();
    saved_max_reinforcement = user->GetMaxReinforcement();
    saved = true;
}
void BrittleEfficiency::UseBindingVow() {
    parent->SetMaxReinforcement(saved_max_reinforcement * 0.5);
    parent->SetReinforcementCostMult(saved_reinforcement_cost * 0.45);
}
void BrittleEfficiency::RemoveEffects() {
    parent->SetMaxReinforcement(saved_max_reinforcement);
    parent->SetReinforcementCostMult(saved_reinforcement_cost);
}
std::unique_ptr<BindingVow> BrittleEfficiency::Clone() const {
    return std::make_unique<BrittleEfficiency>(*this);
}