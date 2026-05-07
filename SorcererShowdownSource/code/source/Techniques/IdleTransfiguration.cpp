#include "code/header/Techniques/IdleTransfiguration.h"
#include "code/header/Characters/CurseUsers/CursedSpirits/TransfiguredHuman.h"
#include "code/header/GameManagement/BattlefieldHeader.h"
#include "code/header/Characters/Character.h"
#include "code/header/Characters/CurseUsers/CurseUser.h"
#include "code/header/GameManagement/Utils.h"



IdleTransfiguration::IdleTransfiguration() {
    tech_name = "Idle Transfiguration";
    tech_color = "\033[35m";
}


void IdleTransfiguration::UseTransfiguration(CurseUser* user, Character* target) {
    println("{} tried to transfigure {}!", user->GetNameWithID(), target->GetNameWithID());
    double dmg = CalculateDamage(user, transfiguration_damage);
    target->Damage(dmg);
    if (target->GetCharacterHealth() <= 0.0) {
        transfigured_human_count++;
        std::println("{} has gained a transfigured human body", user->GetNameWithID());
    }
    chant = ChantLevel::Zero;
}

void IdleTransfiguration::SummonTransfiguredHumans(Battlefield& bf) {
    int c = 0;
    if (transfigured_human_count > 0) {
        bf.spawn_queue.push_back(std::make_unique<TransfiguredHuman>());
        transfigured_human_count--;
        c++;
    }
    std::println("{} Transfigured humans have been put into the battlefield!", c);
}

void IdleTransfiguration::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (user->DomainAmplificationActive()) {
        std::println("{}You cannot use your innate technique due to domain amplification!{}", Color::Red, Color::Clear);
        return;
    }
    std::println("1 - Use Idle Transfiguration | 2 - Summon Transfigured Humans");
    

    std::print("=> ");
    size_t choice = GetValidInput();
    switch (choice) {
    case 1:
        UseTransfiguration(user, target);
        break;
    case 2:
        GetTransfiguredSummonAmount(bf);
        break;
    default:
        std::println("Invalid Choice");
        break;
    }
}

void IdleTransfiguration::GetTransfiguredSummonAmount(Battlefield& bf) {
    if (transfigured_human_count == 0){
        std::println("You dont have any Transfigured Humans in stock!");
        return;
    }   
    std::println("You currently have {} Transfigured Humans in stock", transfigured_human_count);
    std::println("How much would you like to use\n=> ");
    int ch = GetValidInput();
    int pr = 0;

    while (pr < ch && this->GetTFcount() > 0) {
        SummonTransfiguredHumans(bf);
        pr++;
    }
    std::println("You have summoned {} Transfigured Humans!", pr);
}

int IdleTransfiguration::GetTFcount() const {
    return transfigured_human_count;
}

void IdleTransfiguration::Chant() {}

void IdleTransfiguration::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("===== Idle Transfiguration Menu =====");
    std::println("Transfigured human amount in storage: {}", transfigured_human_count);

}

std::unique_ptr<Technique> IdleTransfiguration::Clone() const {
    return std::make_unique<IdleTransfiguration>(*this);
}

void IdleTransfiguration::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    if (transfigured_human_count > 3) {
        while(transfigured_human_count > 0){
            SummonTransfiguredHumans(bf);
        }
        return;
    }
    UseTransfiguration(user, target);
}