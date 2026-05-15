#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/CursedTools/InvertedSpearOfHeaven.h"
#include "code/header/GameManagement/Utils.h"



InvertedSpearofHeaven::InvertedSpearofHeaven() {
	tool_name = "The Inverted Spear of Heaven";
	tool_color = "\033[33m";
}

void InvertedSpearofHeaven::UseTool(Character* user, Character* target) {
	target->DamageBypass(special_tool_damage * special_tool_mult);
	std::println("{} {}attacks{} {} with {}", user->GetNameWithID(),Utilities::Color::Red,Utilities::Color::Clear, target->GetNameWithID(), this->GetName());
}

std::unique_ptr<CursedTool> InvertedSpearofHeaven::Clone() const {
	return std::make_unique<InvertedSpearofHeaven>(*this);
}

bool InvertedSpearofHeaven::IsAntiTechniqueWeapon() const {
	return true;
}