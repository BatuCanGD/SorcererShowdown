#include "PhysicallyGifted.h"
#include "CursedTool.h"
#include "Limitless.h"
#include "Sorcerer.h"
#include "Utils.h"

import std;

PhysicallyGifted::PhysicallyGifted(double hp, double str) 
	: Character(hp) 
{
	is_heavenly_restricted = true;
	strength = str;
}

std::unique_ptr<Character> PhysicallyGifted::Clone() const {
    auto pg = std::make_unique<PhysicallyGifted>(max_health, base_attack_damage);

    if (this->cursed_tool) pg->SetEquippedTool(this->cursed_tool->Clone());

    pg->SetCharacterName(this->char_name, this->name_color);
    pg->SetBaseDamage(this->base_attack_damage);    
    
    for (const auto& tool : this->inventory_curse) {
        if (tool) pg->AddToolToInventory(tool->Clone());
    }

    return pg;
}

double PhysicallyGifted::GetHealingFactor() const {
    return strength / 15;
}

void  PhysicallyGifted::Attack(Character* target) {
    if (auto* target_cuser = dynamic_cast<CurseUser*>(target)) {
        if (auto* limitless = dynamic_cast<Limitless*>(target_cuser->GetTechnique())) {
            bool has_spear = cursed_tool && cursed_tool->GetSimpleName() == "The Inverted Spear of Heaven";
            if (limitless->CheckInfinity() && !has_spear) {
                std::println("{}'s attack was blocked by {}'s {}Infinity{}!",
                    this->GetNameWithID(), target_cuser->GetNameWithID(), Color::Cyan, Color::Clear);
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
