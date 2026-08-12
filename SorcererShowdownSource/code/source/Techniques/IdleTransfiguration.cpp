#include "Techniques/IdleTransfiguration.h"
#include "Characters/CurseUsers/CursedSpirits/TransfiguredHuman.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"



IdleTransfiguration::IdleTransfiguration() {
    name = "Idle Transfiguration";
    color = "\033[35m";
}

void IdleTransfiguration::UseTransfiguration(CurseUser* user, Character* target) {
    std::println("{} tried to transfigure {}!", user->GetNameWithID(), target->GetNameWithID());
    target->DamageBypassReinforcement(CalculateOutput(user, transfiguration_damage));
    if (target->GetCharacterHealth() <= 0.0 && !target->IsaCursedSpirit()) {
        transfigured_human_count++;
        std::println("{} has gained a transfigured human body", user->GetNameWithID());
    }
    chant = ChantLevel::Zero;
}

void IdleTransfiguration::SummonTransfiguredHumans(Battlefield& bf) {
    if (transfigured_human_count > 0) {
        bf.spawn_queue.push_back(std::make_unique<TransfiguredHuman>());
        transfigured_human_count--;
    }
    std::println("a Transfigured human has been put into the battlefield!");
}

bool IdleTransfiguration::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    std::println("1 - Use Idle Transfiguration | 2 - Summon Transfigured Humans | 3 - Return\n=> ");

    switch (Utilities::GetInput<int>()) {
    case 1:
        UseTransfiguration(user, target);
        return true;
    case 2:
        GetTransfiguredSummonAmount(bf);
        return true;
    case 3:
        break;
    default:
        std::println("Invalid Choice");
    }
    return false;
}

void IdleTransfiguration::GetTransfiguredSummonAmount(Battlefield& bf) {
    if (transfigured_human_count == 0){
        std::println("You dont have any Transfigured Humans in stock!");
        return;
    }   
    std::println("You currently have {} Transfigured Humans in stock", transfigured_human_count);
    std::println("How much would you like to use\n=> ");
    int ch = Utilities::GetInput<int>();
    int pr = 0;

    while (pr < ch && GetTFcount() > 0) {
        SummonTransfiguredHumans(bf);
        pr++;
    }
    std::println("You have summoned {} Transfigured Humans!", pr);
}

int IdleTransfiguration::GetTFcount() const {
    return transfigured_human_count;
}

void IdleTransfiguration::Chant() {}

bool IdleTransfiguration::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("===== Idle Transfiguration Menu =====");
    std::println("Transfigured human amount in storage: {}", transfigured_human_count);
    return false;
}

std::unique_ptr<Technique> IdleTransfiguration::Clone() const {
    return std::make_unique<IdleTransfiguration>(*this);
}

bool IdleTransfiguration::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (transfigured_human_count > 3) {
        while(transfigured_human_count > 0){
            SummonTransfiguredHumans(bf);
        }
        return true;
    }
    UseTransfiguration(user, target);
    return true;
}