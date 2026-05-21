#include "code/header/Characters/CurseUsers/Sorcerers/Sorcerer.h"
#include "code/header/CursedTools/InvertedSpearOfHeaven.h"
#include "code/header/GameManagement/Utils.h"



InvertedSpearofHeaven::InvertedSpearofHeaven() {
	name = "The Inverted Spear of Heaven";
	color = "\033[33m";
	type = WeaponType::TechniqueBypass;
	base_damage = 50.0;
}

std::unique_ptr<CursedTool> InvertedSpearofHeaven::Clone() const {
	return std::make_unique<InvertedSpearofHeaven>(*this);
}