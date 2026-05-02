#include "Sorcerer.h"
#include "InvertedSpearOfHeaven.h"
#include "Utils.h"



InvertedSpearofHeaven::InvertedSpearofHeaven() {
	tool_name = "The Inverted Spear of Heaven";
	tool_color = "\033[33m";
}

void InvertedSpearofHeaven::UseTool(Character* user, Character* target) {
	target->DamageBypass(special_tool_damage * special_tool_mult);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(),Color::Red,Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> InvertedSpearofHeaven::Clone() const {
	return std::make_unique<InvertedSpearofHeaven>(*this);
}

bool InvertedSpearofHeaven::IsAntiTechniqueWeapon() const {
	return true;
}