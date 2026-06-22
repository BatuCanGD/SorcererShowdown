#include "code/header/Characters/Shikigami/Mahoraga.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Colors.h"



Mahoraga::Mahoraga() : Shikigami() {
    name = "Mahoraga";
    color = "\033[33m";
}

void Mahoraga::Adapt() {
    if (!IsActive()) return;

    if (active_turn_amount >= 40) {
        InfStage = InfinityAdaptation::FourthSpin;
    }
    else if (active_turn_amount >= 25) {
        InfStage = InfinityAdaptation::ThirdSpin;
    }
    else if (active_turn_amount >= 15) {
        InfStage = InfinityAdaptation::SecondSpin;
    }
    else if (active_turn_amount >= 5) {
        InfStage = InfinityAdaptation::FirstSpin;
    }
    else {
        InfStage = InfinityAdaptation::None;
    }

}

void Mahoraga::PrintStatus(CurseUser* s) const {
    if (PrevState != InfStage) {
        switch (InfStage) {
        case InfinityAdaptation::None: break;
        case InfinityAdaptation::FirstSpin: std::println("{}'s {} has started to adapt space itself!!!!!", s->GetNameWithID(), GetName()); break;
        case InfinityAdaptation::SecondSpin: std::println("{}'s {} is on its second spin to adapt space itself!!!!!", s->GetNameWithID(), GetName()); break;
        case InfinityAdaptation::ThirdSpin: std::println("{}'s {} is on its final spin to adapt space itself!!!!!",s->GetNameWithID(), GetName()); break;
        case InfinityAdaptation::FourthSpin: std::println("{}'s {} has adapted to space itself!!!!!", s->GetNameWithID(), GetName()); break;
        default:
            break;
        }
    }
}

void Mahoraga::UpdatePreviousState() {
    if (PrevState != InfStage) {
        PrevState = InfStage;
    }
}

bool Mahoraga::FullyAdapted() const {
    return InfStage == InfinityAdaptation::FourthSpin;
}

void Mahoraga::OnShikigamiTurn(CurseUser* user, Battlefield&) {
    if (!IsActive()) {
        Regen(shadow_health_regen);
        return;
    }
    if (user->GetCharacterCE() < keep_active_cost) {
        std::println("{} cannot maintain its active state due to {}'s insufficient {}Cursed Energy!{} It withdraws back into the shadows",GetName(), user->GetNameWithID(), Color::Cyan, Color::Clear);
        Withdraw();
        return;
    }
    ActiveTimeIncrementor();
    Adapt();
    PrintStatus(user);
    UpdatePreviousState();
    user->SpendCE(keep_active_cost);
}

bool Mahoraga::IsMahoraga() const {
    return true;
}