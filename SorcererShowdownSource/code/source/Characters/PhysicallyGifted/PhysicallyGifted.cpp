#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Techniques/Limitless.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



PhysicallyGifted::PhysicallyGifted(double hp, double str) 
	: Character(hp) 
{
	strength = str;
}

std::unique_ptr<Character> PhysicallyGifted::Clone() const {
    auto pg = std::make_unique<PhysicallyGifted>(max_health, strength);
    if (this->cursed_tool) pg->SetEquippedTool(this->cursed_tool->Clone());
    pg->SetCharacterName(this->char_name, this->name_color);
    pg->SetStrength(this->strength);
    pg->SetBaseDamage(this->base_attack_damage);    

    if (this->brain) pg->SetBrain(this->brain->Clone());

    for (const auto& tool : this->inventory_curse) {
        if (tool) pg->AddToolToInventory(tool->Clone());
    }
    return pg;
}

double PhysicallyGifted::GetHealingFactor() const {
    return strength / 15;
}

void  PhysicallyGifted::Attack(Character* target) {
    if (target->IsaCurseUser()) {
        auto* target_cuser = static_cast<CurseUser*>(target);
        if (auto* tech = target_cuser->GetTechnique()) {
            bool has_spear = cursed_tool && cursed_tool->IsAntiTechniqueWeapon();
            if ((tech->IsLimitless() && tech->IsInfinityActive()) && !has_spear) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!",
                    this->GetNameWithID(), target_cuser->GetNameWithID(), Utilities::Color::Cyan, Utilities::Color::Clear);
                return;
            }
        }
    }

    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
    }
    else {
        std::println("{} strikes {} with their bare hands!", this->GetNameWithID(), target->GetNameWithID());
        target->Damage(10.0 * this->GetStrengthDamage());
    }
}

bool PhysicallyGifted::IsPhysicallyGifted() const {
	return true;
}

double PhysicallyGifted::GetStrength() const {
	return strength;
}
double PhysicallyGifted::GetStrengthDamage()const {
	return 1.0 + (strength / strength_to_damage_ratio);
}

double PhysicallyGifted::GetDamageReinforcement() const {
	return 1.0 + (strength / strength_to_reinforcement_ratio);
}

void PhysicallyGifted::SetStrength(double str) {
    strength = str;
}

void PhysicallyGifted::TickCharacterSpecialty() {
    this->Regen(this->GetHealingFactor());
}