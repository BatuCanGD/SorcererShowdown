#include "Techniques/Shrine.h"
#include "Characters/CurseUsers/CurseUser.h"
#include "GameManagement/BattlefieldHeader.h"
#include "GameManagement/Colors.h"
#include "GameManagement/Utils.h"

Shrine::Shrine() {
    name = "Shrine";
    color = "\033[31m";
}


void Shrine::UseDismantle(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) {
        std::println("{}\"MAXIMUM OUTPUT: DISMANTLE!\"{}", Color::BrightRed, Color::Clear);
    }
    std::println("{} uses {}Dismantle{} on {}!", user->GetNameWithID(),Color::BrightRed,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateOutput(user, dsm_dmg)); 
    chant = ChantLevel::Zero;
}

void Shrine::UseCleave(CurseUser* user, Character* target) {
    if (chant == ChantLevel::Four) {
        std::println("{}\"MAXIMUM OUTPUT: CLEAVE!\"{}", Color::Red, Color::Clear);
    }
    std::println("{} uses {}Cleave{} on {}!", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID());
    target->Damage(CalculateOutput(user, dsm_dmg));
    chant = ChantLevel::Zero;
}

void Shrine::UseSpiderweb(CurseUser* user, Battlefield& bf) {
    if (chant < ChantLevel::One){
        std::println("You need a higher output to use this move");
        return;
    }
    std::println("{}\\/\\/\\/\\/\\/\\\\/\\/\\/\\/\\/\\\\/\\/\\/\\/\\/\\/\\/\\/\\\n"
                 "\\/\\/\\/\\/\\/\\CLEAVE: SPIDERWEB\\/\\/\\/\\/\\/\\\n"
                 "\\/\\/\\/\\/\\/\\\\/\\/\\/\\/\\/\\\\/\\/\\/\\/\\/\\/\\/\\/\\{}\n",
                Color::Red, Color::Clear);
    const double dmg = CalculateOutput(user, web_dmg);
    for (const auto& c : bf.battlefield){
        if (c.get() == user) continue;
        c->SetStunState(true);
        c->Damage(dmg);
    }
    chant = ChantLevel::Zero;
}



bool Shrine::TechniqueMenu(CurseUser* user, Character* target, Battlefield& bf) {
    if (chant >= ChantLevel::One) {
        std::println("1 - Use Dismantle | 2 - Use Cleave | 3 - Use Spiderwebbed Cleave");
    }else{
        std::println("1 - Use Dismantle | 2 - Use Cleave");
    }
    std::print("=> ");
    
    switch (Utilities::GetInput<int>()) {
    case 1:
        UseDismantle(user, target);
        return true;
    case 2:
        UseCleave(user, target);
        return true;
    case 3:
        UseSpiderweb(user, bf);
        return true;
    default:
        std::println("Invalid Input");
    }
    return false;
}

bool Shrine::TechniqueSetting(CurseUser*, Battlefield&) {
    std::println("Chant level: [{}]", GetStringChantLevel());
    std::println("1 - Chant | 2 - Return");
    std::print("=> ");
    int ch = Utilities::GetInput<int>();
    switch (ch) {
    case 1:
        Chant();
        return true;
    case 2:
        break;
    default:
        std::println("Invalid Input");
    }
    return false;
}

void Shrine::Chant() {
    if (chant == ChantLevel::Zero) {
        std::println("{}\"Scale of the Dragon\"{}",Color::BrightRed,Color::Clear);
        chant = ChantLevel::One;
    }
    else if (chant == ChantLevel::One) {
        std::println("{}\"Recoil.\"{}", Color::BrightRed, Color::Clear);
        chant = ChantLevel::Two;
    }
    else if (chant == ChantLevel::Two) {
        std::println("{}\"Twin Meteors.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Three;
    }
    else if (chant == ChantLevel::Three) {
        std::println("{}\"Silence. Desolation. Anatomy of Ruin.\"{}", Color::Red, Color::Clear);
        chant = ChantLevel::Four;
    }
    else {
        std::println("The technique is already at maximum output. Sever them!");
    }
}

bool Shrine::AutoTechniqueUse(CurseUser* user, Character* target, Battlefield& bf) {
    int clv_roll = Utilities::GetRandom(1, 30);
    if (clv_roll >= 25) {
        UseCleave(user, target);
        return true;
    }else if (clv_roll >= 20 && chant >= ChantLevel::One){
        UseSpiderweb(user, bf);
        return true;
    }
    Specials* sp = user->GetSpecial();
    if (sp && sp->IsWorldCuttingSlash() && sp->CheckSpecial(user)) {
        if (chant == ChantLevel::Four) {
            sp->UseSpecial(user, target, bf);
            return true;
        }
    }
    if (Utilities::GetRandom(1, 10) >= 6 || (sp && sp->CheckSpecial(user))) {
        Chant();
        return true;
    }
    else {
        UseDismantle(user, target);
        return true;
    } 
}

std::unique_ptr<Technique> Shrine::Clone() const {
    return std::make_unique<Shrine>(*this);
}

bool Shrine::IsShrine() const {
    return true;
}