#include "code/header/Characters/PhysicallyGifted/PhysicallyGifted.h"
#include "code/header/CursedTools/CursedTool.h"
#include "code/header/Techniques/Limitless/Limitless.h"
#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/GameManagement/Utils.h"



PhysicallyGifted::PhysicallyGifted(double hp, double str) : 
    Character(hp), 
    strength(str) {
}

std::unique_ptr<Character> PhysicallyGifted::Clone() const {
    auto pg = std::make_unique<PhysicallyGifted>(max_health, strength);
    if (this->brain) pg->SetBrain(this->brain->Clone());
    if (this->cursed_tool) pg->SetEquippedTool(this->cursed_tool->Clone());
    pg->SetCharacterName(this->name, this->color);
    pg->SetStrength(this->strength);
    pg->SetBaseDamage(this->base_attack_damage);    
    for (const auto& tool : this->inventory_curse) {
        if (tool) pg->AddToolToInventory(tool->Clone());
    }
    return pg;
}

double PhysicallyGifted::GetHealingFactor() const {
    return strength / 30.0;
}

void  PhysicallyGifted::Attack(Character* target) {
    if (cursed_tool) {
        cursed_tool->UseTool(this, target);
        return;
    }
    if (target->IsaCurseUser()) {
        auto* tc = static_cast<CurseUser*>(target);
        if (tc->GetTechnique() && tc->GetTechnique()->IsLimitless() && tc->GetTechnique()->IsInfinityActive()) {
            std::println("{}'s attack was blocked by {}'s {}Infinity{}!",
                this->GetNameWithID(), tc->GetNameWithID(), Utilities::Color::Cyan, Utilities::Color::Clear);
            return;
        }
    }
    std::println("{} strikes {} with their bare hands!", this->GetNameWithID(), target->GetNameWithID());
    target->Damage(10.0 * this->GetStrengthDamage());
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